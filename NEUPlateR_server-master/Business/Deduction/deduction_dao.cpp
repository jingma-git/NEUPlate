#include "deduction_dao.h"

//initial static variable
QString CDeductionDAO::DEDUCTION_CHARGE_TYPE = "DEDUCTION_CHARGE_TYPE";
QString CDeductionDAO::DEDUCTION_CHARGE_PRICE = "DEDUCTION_CHARGE_PRICE";
QString CDeductionDAO::DEDUCTION_BASIC_PRICE = "DEDUCTION_BASIC_PRICE";
QString CDeductionDAO::DEDUCTION_STATE_REVERSAL = "DEDUCTION_STATE_REVERSAL";

/**
* @name          CDeductionDAO
* @brief         class CDeductionDAO constructor that get database connection from connection pool.
* @author        luxijia
* @date          2018-07-27
*/
CDeductionDAO::CDeductionDAO():
    m_redis("localhost")
{

}

/**
* @name          CDeductionDAO
* @brief         class CDeductionDAO constructor that .
* @author        luxijia
* @date          2018-07-27
* @param         db {@QSqlDatebase} object
*/
CDeductionDAO::CDeductionDAO(const QSqlDatabase &db):
    m_db(db),
    m_redis("localhost")
{

}

/**
* @name          ~CDeductionDAO
* @brief         class CDeductionDAO deconstructor that return database connection from connection pool.
* @author        luxijia
* @date          2018-07-27
*/
CDeductionDAO::~CDeductionDAO()
{
    ConnectionPool::close_connection(m_db);
}

/**
* @name          insert_deduction
* @brief         inseat deduction record into database.
* @author        luxijia
* @date          2018-07-27
* @param         deduction_id deduction record id.
* @param         deduction_time deduction time.
* @param         drive_record drive record id.
* @param         pay_
* @param         end the end toll station id.
* @return        SUCCESS is insert Deduction record success.
*                SQL_EXEC_ERROR is execute sql statement error.
*/
int CDeductionDAO::insert_deduction_record(const QString &deduction_id, const QString &deduction_time,
                                           const QString &drive_record, const QString &plate, const int pay_method, const double fee)
{
    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);

    query.prepare("INSERT INTO fee_deduction VALUES(:deduction_id, :drive_record, :fee, :pay_method,\
                  :deduction_time, :plate, NULL)");
    query.bindValue(":deduction_id", deduction_id);
    query.bindValue(":drive_record", drive_record);
    query.bindValue(":fee", fee);
    query.bindValue(":pay_method", pay_method);
    query.bindValue(":deduction_time", deduction_time);
    query.bindValue(":plate", plate);

    if (!query.exec())
    {
        Log::error(QString("%1%2").arg("Insert deduction record failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }
    m_db.commit();
    return StatusCode::SUCCESS;
}

/**
* @name          insert_deduction_record_by_payer
* @brief         inseat deduction record into database.
* @author        luxijia
* @date          2018-07-27
* @param         deduction_id deduction record id.
* @param         deduction_time deduction time.
* @param         drive_record drive record id.
* @param         pay_
* @param         end the end toll station id.
* @return        SUCCESS is insert deduction record success.
*                SQL_EXEC_ERROR is execute sql statement error.
*                INSERT_ERROR is insert deduction failed.
*/
int CDeductionDAO::insert_deduction_record_by_payer(const QString &payer_id, const QString &deduction_id, const QString &deduction_time,
                                                    const QString &drive_record, const QString &plate, const int pay_method, const double fee)
{   
    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);
    CPayerDao payer_dao(m_db);
    double balance = 0;
    int code;

    m_db.transaction();
    code = payer_dao.quert_balance_by_payer_id(payer_id, balance);

    if(StatusCode::SUCCESS != code)
    {
        m_db.rollback();
        Log::error("Query payer balance error.");
        return StatusCode::INSERT_ERROR;
    }

    if (balance < fee)
    {
        m_db.rollback();
        return StatusCode::FEE_NO_ENOUGH;
    }

    query.prepare("INSERT INTO fee_deduction VALUES(:deduction_id, :drive_record, :fee, :pay_method,\
                  :deduction_time, :plate, :payer_id)");
    query.bindValue(":deduction_id", deduction_id);
    query.bindValue(":drive_record", drive_record);
    query.bindValue(":fee", fee);
    query.bindValue(":pay_method", pay_method);
    query.bindValue(":deduction_time", deduction_time);
    query.bindValue(":payer_id", payer_id);
    query.bindValue(":plate", plate);

    if (!query.exec())
    {
        m_db.rollback();
        Log::error(QString("%1%2").arg("Insert Deduction record failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    code = payer_dao.add_balance_by_payer_id(payer_id, -fee);

    if (StatusCode::SUCCESS != code)
    {
        m_db.rollback();
        Log::error("Detucte payer balance error.");
        return StatusCode::INSERT_ERROR;
    }

    m_db.commit();
    return StatusCode::SUCCESS;
}

/**
* @name          insert_reversal_deduction_record
* @brief         insert dedtuction reversal record to database.
* @author        luxijia
* @date          2018-07-31
* @param         deduction_id the deduction record id
* @return        SUCCESS is insert reversal deduction record success.
*                SQL_EXEC_ERROR is execute sql statement error.
*                INSERT_ERROR is insert reversal deduction record failed.
*                TIME_OUT_ERROR is reversal deduction record time out.
*/
int CDeductionDAO::insert_reversal_deduction_record(const QString &deduction_id, const QString &new_deduction_id)
{
//    std::string state_to_reversal = DEDUCTION_STATE_REVERSAL.toStdString() + "_" + deduction_id;

    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);

    query.prepare("CALL add_reversal_record(:deduction_id, :new_deduction_id, @result)");
    query.bindValue(":deduction_id", deduction_id);
    query.bindValue(":new_deduction_id", new_deduction_id);

    if (!query.exec())
    {
        Log::error(QString("%1%2").arg("Insert reversal deduction record failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (!query.exec("SELECT @result"))
    {
        Log::error(QString("%1%2").arg("Query reversal deduction record result failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (!query.next())
        return StatusCode::SQL_EXEC_ERROR;

    if (1 == query.value(0).toInt())
        return StatusCode::INSERT_ERROR;
    else if (2 == query.value(0).toInt())
        return StatusCode::TIME_OUT_ERROR;
    else if (3 == query.value(0).toInt())
        return  StatusCode::HAS_REVERSAL;

    return StatusCode::SUCCESS;
}

/**
* @name          insert_reversal_deduction_record_with_account
* @brief         insert dedtuction reversal record to database and
*                deduct account money.
* @author        luxijia
* @date          2018-07-31
* @param         deduction_id the deduction record id
* @return        SUCCESS is insert reversal deduction record success.
*                SQL_EXEC_ERROR is execute sql statement error.
*                INSERT_ERROR is insert reversal deduction record failed.
*                TIME_OUT_ERROR is reversal deduction record time out.
*/
int CDeductionDAO::insert_reversal_deduction_record_with_account(const QString &deduction_id, const QString &reversal_deduction_one, const QString &reversal_deduction_two, const QString &plate, const QString &payer_id)
{
    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);
    CPayerDao payer_dao(m_db);
    int code;

    m_db.transaction();
    query.prepare("CALL reversal_deduction_with_account(:deduction_id, :reversal_one, :reversal_two,\
                   :payer_id, :plate, @result, @money, @old_payer_id)");
    query.bindValue(":deduction_id", deduction_id);
    query.bindValue(":reversal_one", reversal_deduction_one);
    query.bindValue(":reversal_two", reversal_deduction_two);
    query.bindValue(":payer_id", payer_id);
    query.bindValue(":plate", plate);

    if (!query.exec())
    {
        m_db.rollback();
        Log::error(QString("%1%2").arg("Insert reversal deduction record failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (!query.exec("SELECT @result, @money, @old_payer_id"))
    {
        m_db.rollback();
        Log::error(QString("%1%2").arg("Query reversal deduction record result failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (!query.next())
    {
        m_db.rollback();
        return StatusCode::SQL_EXEC_ERROR;
    }

    code = query.value(0).toInt();

    if (1 == code)
    {
        m_db.rollback();
        return StatusCode::INSERT_ERROR;
    }
    else if (2 == code)
    {
        m_db.rollback();
        return StatusCode::TIME_OUT_ERROR;
    }
    else if (3 == code)
    {
        m_db.rollback();
        return  StatusCode::HAS_REVERSAL;
    }

    QString old_payer_id = query.value(2).toString();
    double money = query.value(1).toDouble();

    if (old_payer_id.isEmpty() || 0 == money)
    {
        m_db.rollback();
        return StatusCode::INSERT_ERROR;
    }

    code = payer_dao.add_balance_by_payer_id(old_payer_id, money);

    if (StatusCode::SUCCESS != code)
    {
        m_db.rollback();
        Log::warning("return money error");
        return StatusCode::INSERT_ERROR;
    }

    code = payer_dao.add_balance_by_payer_id(payer_id, -money);

    if (StatusCode::SUCCESS != code)
    {
        m_db.rollback();
        Log::warning("deduct money error");
        return StatusCode::INSERT_ERROR;
    }

    m_db.commit();
    return StatusCode::SUCCESS;
}

int CDeductionDAO::insert_reversal_deduction_record_no_account(const QString &deduction_id, const QString &reversal_deduction_one, const QString &reversal_deduction_two)
{
    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);
    CPayerDao payer_dao(m_db);
    int code;

    m_db.transaction();
    query.prepare("CALL reversal_deduction_with_no_account(:deduction_id, :reversal_one, :reversal_two,\
                   @result, @money, @old_payer_id)");
    query.bindValue(":deduction_id", deduction_id);
    query.bindValue(":reversal_one", reversal_deduction_one);
    query.bindValue(":reversal_two", reversal_deduction_two);

    if (!query.exec())
    {
        m_db.rollback();
        Log::error(QString("%1%2").arg("Insert reversal deduction record failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (!query.exec("SELECT @result, @money, @old_payer_id"))
    {
        m_db.rollback();
        Log::error(QString("%1%2").arg("Query reversal deduction record result failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (!query.next())
    {
        m_db.rollback();
        return StatusCode::SQL_EXEC_ERROR;
    }

    code = query.value(0).toInt();

    if (1 == code)
    {
        m_db.rollback();
        return StatusCode::INSERT_ERROR;
    }
    else if (2 == code)
    {
        m_db.rollback();
        return StatusCode::TIME_OUT_ERROR;
    }
    else if (3 == code)
    {
        m_db.rollback();
        return  StatusCode::HAS_REVERSAL;
    }

    double money = query.value(1).toDouble();
    QString old_payer_id = query.value(2).toString();

    if (old_payer_id.isEmpty() || 0 == money)
    {
        m_db.rollback();
        return StatusCode::INSERT_ERROR;
    }

    code = payer_dao.add_balance_by_payer_id(old_payer_id, money);

    if (StatusCode::SUCCESS != code)
    {
        m_db.rollback();
        Log::warning("return money error");
        return StatusCode::INSERT_ERROR;
    }

    m_db.commit();
    return StatusCode::SUCCESS;
}

/**
* @name          insert_correct_error_plate
* @brief         insert correct record
* @author        luxijia
* @date          2018-08-08
* @param         deduction_id the id of reversal deduction record.
* @param         plate the correct plate number.
* @return        SUCCESS is insert correct plate record success.
*                SQL_EXEC_ERROR is execute sql statement error.
*                INSERT_ERROR is insert correct plate record failed.
*/
int CDeductionDAO::insert_correct_error_plate(const QString &recognition_id, const QString &plate)
{
    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);

    m_db.transaction();
    query.prepare("CALL add_correct_plate(:recognition_id, :plate, @result)");
    query.bindValue(":recognition_id", recognition_id);
    query.bindValue(":plate", plate);

    if (!query.exec())
    {
        m_db.rollback();
        Log::error(QString("%1%2").arg("Insert correct plate record failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (!query.exec("SELECT @result"))
    {
        m_db.rollback();
        Log::error(QString("%1%2").arg("Query correct plate record result failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (!query.next())
    {
        m_db.rollback();
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (1 == query.value(0).toInt())
    {
        m_db.rollback();
        return StatusCode::INSERT_ERROR;
    }

    m_db.commit();
    return StatusCode::SUCCESS;
}

/**
* @name          query_charge_type
* @brief         query high way charge type.
* @author        luxijia
* @date          2018-07-27
* @param         charge_type highway charge type.
* @param         car_class the class type of car.
* @param         highway_id the id of highway.
* @return        SUCCESS is query charge type success.
*                SQL_EXEC_ERROR is execute sql statement error.
*                EMPTY_QUERY is query charge type failed.
*/
int CDeductionDAO::query_charge_type(int &charge_type, const QString &car_type, const QString &highway_id)
{
    //Get data from redis if it has
    std::string type = DEDUCTION_CHARGE_TYPE.toStdString() + "_" + highway_id.toStdString() +
                              "_" + car_type.toStdString();

    if (m_redis.exists(type))
    {
        charge_type = QString::fromStdString(m_redis.get(type)).toInt();
        return StatusCode::SUCCESS;
    }

    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    //query form remote database server
    QSqlQuery query(m_db);

    query.prepare("SELECT charge_method FROM charge_standard WHERE highway_id = :highway_id AND `class` = :car_class");
    query.bindValue(":highway_id", highway_id);
    query.bindValue(":car_class", car_type);

    if (!query.exec())
    {
        Log::error(QString("%1%2").arg("Query charge type failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (!query.next())
        return StatusCode::EMPTY_QUERY;

    charge_type = query.value("charge_method").toInt();
    m_redis.set(type, std::to_string(charge_type));
    return StatusCode::SUCCESS;
}

/**
* @name          query_charge_price
* @brief         query high way charge fee.
* @author        luxijia
* @date          2018-07-27
* @param         price query highway chagre fee result.
* @param         car_type the type of car.
* @param         highway_id the id of highway.
* @return        SUCCESS is query highway charge price success.
*                SQL_EXEC_ERROR is execute sql statement error.
*                EMPTY_QUERY is query highway charge price failed.
*/
int CDeductionDAO::query_charge_price(double &price, const QString &car_type, const QString &highway_id)
{
    //Get data from redis if it has
    std::string charge_price = DEDUCTION_CHARGE_PRICE.toStdString() + "_" + highway_id.toStdString() +
                              "_" + car_type.toStdString();

    if (m_redis.exists(charge_price))
    {
        price = QString::fromStdString(m_redis.get(charge_price)).toDouble();
        return StatusCode::SUCCESS;
    }

    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    //query form remote database server
    QSqlQuery query(m_db);

    query.prepare("SELECT price FROM charge_standard WHERE highway_id = :highway_id AND `class` = :car_class");
    query.bindValue(":highway_id", highway_id);
    query.bindValue(":car_class", car_type);

    if (!query.exec())
    {
        Log::error(QString("%1%2").arg("Query highway charge price failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (!query.next())
        return StatusCode::EMPTY_QUERY;

    price = query.value("price").toDouble();
    m_redis.set(charge_price, std::to_string(price));
    return StatusCode::SUCCESS;
}

/**
* @name          query_basic_price
* @brief         query high way basic charge fee.
* @author        luxijia
* @date          2018-07-27
* @param         basic_price query highway basic chagre fee result.
* @param         car_type the type of car.
* @param         highway_id the id of highway.
* @return        SUCCESS is query basic price success.
*                SQL_EXEC_ERROR is execute sql statement error.
*                EMPTY_QUERY is query basic price failed.
*/
int CDeductionDAO::query_basic_price(double &basic_price, const QString &car_type, const QString &highway_id)
{
    //Get data from redis if it has
    std::string basic = DEDUCTION_BASIC_PRICE.toStdString() + "_" + highway_id.toStdString() +
                              "_" + car_type.toStdString();

    if (m_redis.exists(basic))
    {
        basic_price = QString::fromStdString(m_redis.get(basic)).toDouble();
        return StatusCode::SUCCESS;
    }

    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    //query form remote database server
    QSqlQuery query(m_db);

    query.prepare("SELECT basic_price FROM charge_standard WHERE highway_id = :highway_id AND `class` = :car_class");
    query.bindValue(":highway_id", highway_id);
    query.bindValue(":car_class", car_type);

    if (!query.exec())
    {
        Log::error(QString("%1%2").arg("Query highway basic price failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (!query.next())
        return StatusCode::EMPTY_QUERY;

    basic_price = query.value("basic_price").toDouble();
    m_redis.set(basic, std::to_string(basic_price));
    return StatusCode::SUCCESS;
}

/**
* @name          query_deduction
* @brief         query deduction by keyword, time and state.
* @author        luxijia
* @date          2018-07-31
* @param         records query dedcution record result.
* @param         all_row_number all deduction record row number.
* @param         keyword query deduction record keyword
* @param         start_time dedcution record that be queryed start time.
* @param         end_time dedcution record that be queryed end time.
* @param         state dedcution record state.
* @param         offset the begin postion of query dedcution table.
* @param         query_number query deductoin number.
* @return        SUCCESS is query deduction success.
*                SQL_EXEC_ERROR is exeute sql statement has errors.
*/
int CDeductionDAO::query_deduction(std::vector<CDedutionRecord> &records, int &all_row_number,
                                   const QString &keyword, const QString &start_time,
                                   const QString &end_time, const int state, const int offset,
                                   const int query_number)
{

    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);
    QString state_statement = "";

    if (FINISH == state)
        state_statement = "AND fee >= 0 ";
    else if (REVERSAL == state)
        state_statement = "AND fee < 0 ";

    QString sql = "SELECT SQL_CALC_FOUND_ROWS * FROM deduction_info WHERE deduction_time >= :start_time AND \
                   deduction_time <= :end_time AND\
                   CONCAT(IFNULL(plate, ''),IFNULL(start_name, ''),IFNULL(end_name,'')) \
                   LIKE :keyword " + state_statement + " LIMIT :offset, :query_number";
    query.prepare(sql);
    query.bindValue(":start_time", start_time);
    query.bindValue(":end_time", end_time);
    query.bindValue(":keyword", "%" + keyword +"%");
    query.bindValue(":offset", offset);
    query.bindValue(":query_number", query_number);

    if (!query.exec())
    {
        Log::error(QString("%1%2").arg("Query deduction record failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    while (query.next())
    {
        CDedutionRecord deduction_record;
        deduction_record.setDeduction_id(query.value("id").toString());
        deduction_record.setPlate(query.value("plate").toString());
        deduction_record.setStart_name(query.value("start_name").toString());
        deduction_record.setEnd_name(query.value("end_name").toString());
        deduction_record.setDistance(query.value("distance").toDouble());
        deduction_record.setFee(query.value("fee").toDouble());
        deduction_record.setPay_menthod(query.value("pay_method").toInt());
        deduction_record.setDeduction_time(query.value("deduction_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        records.push_back(deduction_record);
    }

    if (!query.exec("SELECT FOUND_ROWS()"))
    {
        Log::error(QString("%1%2").arg("Query deduction record failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (query.next())
        all_row_number = query.value(0).toInt();

    return StatusCode::SUCCESS;
}

/**
* @name          query_reversal_deduction_validity
* @brief         query reversal deduction validity.
* @author        luxijia
* @date          2018-08-02
* @param         deduction_id the id of deduction record.
* @return        SUCCESS is query reversal deduction vaildity success.
*                SQL_EXEC_ERROR is exeute sql statement has errors.
*                INVALIED_ERROR is query reversal deduction vaildity failed.
*/
int CDeductionDAO::query_reversal_deduction_validity(QString &drive_id, const QString &deduction_id)
{
    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);
    QDateTime deduction_time;

    m_db.transaction();

    query.prepare("SELECT drive_id, deduction_time FROM fee_deduction WHERE id = :deduction_id");
    query.bindValue(":deduction_id", deduction_id);

    if (!query.exec())
    {
        m_db.commit();
        Log::error(QString("%1%2").arg("Query deduction record validity failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (query.next())
    {
        drive_id = query.value("drive_id").toString();
        deduction_time = query.value("deduction_time").toDateTime();
    }

    query.prepare("SELECT * FROM fee_deduction WHERE drive_id = :drive_id AND fee < 0");
    query.bindValue(":drive_id", drive_id);

    if (!query.exec())
    {
        m_db.commit();
        Log::error(QString("%1%2").arg("Query reversal deduction record amount failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    query.prepare("SELECT FOUND_ROWS()");

    if (!query.exec())
    {
        m_db.commit();
        Log::error(QString("%1%2").arg("Query deduction record found rows failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (query.next())
        if (0 != query.value(0).toInt())
        {
            m_db.commit();
            return StatusCode::INVAILD_REVERSAL;
        }

    if (deduction_time.addDays(2) < QDateTime::currentDateTime())
    {
        m_db.commit();
        return StatusCode::TIME_OUT_ERROR;
    }

    m_db.commit();
    return StatusCode::SUCCESS;
}

/**
* @name          delete_deduction_record
* @brief         delete dedcution record by deduction id.
* @author        luxijia
* @date          2018-07-31
* @param         deduction_id the id of deduction.
* @return        SUCCESS is delete deduction record success.
*                SQL_EXEC_ERROR is exeute sql statement has errors.
*/
int CDeductionDAO::delete_deduction_record(const QString &deduction_id)
{
    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);
    QString sql = QString("DELETE FROM fee_deduction WHERE id IN (%1)").arg(deduction_id);

    if (!query.exec(sql))
    {
        Log::error(QString("%1%2").arg("Delete deduction record failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    return StatusCode::SUCCESS;
}
