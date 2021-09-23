#include "travel_record_dao.h"


/**
* @name          CTravelRecordDAO
* @brief         class CTravelRecordDAO constructor that get database connection from connection pool.
* @author        luxijia
* @date          2018-07-27
*/
CTravelRecordDAO::CTravelRecordDAO():
    m_redis("localhost")
{

}

/**
* @name          CTravelRecordDAO
* @brief         class CTravelRecordDAO constructor that .
* @author        luxijia
* @date          2018-07-27
* @param         db {@QSqlDatebase} object
*/
CTravelRecordDAO::CTravelRecordDAO(const QSqlDatabase &db):
    m_db(db),
    m_redis("localhost")
{

}

/**
* @name          ~CTravelRecordDAO
* @brief         class CTravelRecordDAO deconstructor that return database connection from connection pool.
* @author        luxijia
* @date          2018-07-25
*/
CTravelRecordDAO::~CTravelRecordDAO()
{
    ConnectionPool::close_connection(m_db);
}

/**
* @name          insert_start_place
* @brief         add a new drive recognize and insert start place id.
* @author        luxijia
* @date          2018-07-27
* @param         start the start place id
* @return        SQL_EXEC_ERROR is execute sql statement failed.
*                SUCCESS is insert start place success.
*                INSERT_ERROR is insert start place failed.
*/
int CTravelRecordDAO::insert_start_place(const QString &start)
{

    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);
    int result;

    query.prepare("CALL add_start_place(:start, @result)");
    query.bindValue(":start", start);

    if (!query.exec())
    {
        Log::error(QString("%1%2").arg("Insert start place failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (!query.exec("select @result"))
    {
        Log::error(QString("%1%2").arg("Query nsert start place result failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (query.next())
        result = query.value(0).toInt();

    if (0 == result)
        return StatusCode::SUCCESS;

    return StatusCode::INSERT_ERROR;
}

/**
* @name          insert_end_place
* @brief         insert end place id in special drive recoginize where start is start.
* @author        luxijia
* @date          2018-07-27
* @param         start the start place id
* @param         end the end place id
* @return        SQL_EXEC_ERROR is execute sql statement failed.
*                SUCCESS is end start place success.
*                INSERT_ERROR is insert start place failed.
*/
int CTravelRecordDAO::insert_end_place(QString &record_id, const QString &end, const QString &plate)
{
    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);

    m_db.transaction();
    query.prepare("CALL add_end_place(:end, :plate, @result)");

    query.bindValue(":end", end);
    query.bindValue(":plate", plate);

    if (!query.exec())
    {
        m_db.rollback();
        Log::error(QString("%1%2").arg("Insert end place failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (!query.exec("SELECT @result"))
    {
        m_db.rollback();
        Log::error(QString("%1%2").arg("Query insert drive record id failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (query.next())
        record_id = query.value(0).toString();

    if (record_id.isEmpty())
    {
        m_db.rollback();
        return StatusCode::INSERT_ERROR;
    }

    m_db.commit();
    return StatusCode::SUCCESS;
}

/**
* @name          update_drive_distance
* @brief         update drive reocrd distacne
* @author        luxijia
* @date          2018-07-31
* @param         record_id the id of drive record
* @param         distance the distance of drive record
* @return        SQL_EXEC_ERROR is execute sql tatement failed.
*                SUCCESS is upadate drive record distance success.
*/
int CTravelRecordDAO::update_drive_distance(const QString &record_id, const double distance)
{
    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);

    query.prepare("UPDATE drive_record SET distance = :distance WHERE id = :record_id");
    query.bindValue(":record_id", record_id);
    query.bindValue(":distance", distance);
    qDebug() << distance;

    if (!query.exec())
    {
        Log::error(QString("%1%2").arg("Update drive record distance failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    return StatusCode::SUCCESS;
}

/**
* @name          query_drive_record_start
* @brief         query start recognition id according to drive record id.
* @author        luxijia
* @date          2018-07-27
* @param         start the start recognition id.
* @param         drive_record drive record id.
* @return        SQL_EXEC_ERROR is execute sql statement failed.
*                SUCCESS is query drive record start id success.
*                EMPTY_QUERY is query drive record start id failed.
*/
int CTravelRecordDAO::query_drive_record_start(QString &start, const QString &drive_record)
{
    std::string record_start = DRIVE_RECORD_START().toStdString() + "_" + drive_record.toStdString();

    if (m_redis.exists(record_start))
    {
        start = QString::fromStdString(m_redis.get(record_start));
        return StatusCode::SUCCESS;
    }

    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);

    query.prepare("SELECT start FROM drive_record WHERE id = :drive_record");
    query.bindValue(":drive_record", drive_record);

    if (!query.exec())
    {
        Log::error(QString("%1%2").arg("Query drive record start failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (query.next())
        start = query.value(0).toString();

    if (start.isEmpty())
        return StatusCode::EMPTY_QUERY;

    m_redis.set(record_start, start.toStdString());
    return StatusCode::SUCCESS;
}

/**
* @name          query_drive_record_end
* @brief         query end recognition id according to drive record id.
* @author        luxijia
* @date          2018-07-27
* @param         start the start recognition id.
* @param         drive_record drive record id.
* @return        SQL_EXEC_ERROR is execute sql statement failed.
*                SUCCESS is query drive record end id success.
*/
int CTravelRecordDAO::query_drive_record_end(QString &end, const QString &drive_record)
{
    std::string record_end = DRIVE_RECORD_END().toStdString() + "_" + drive_record.toStdString();

    if (m_redis.exists(record_end))
    {
        end = QString::fromStdString(m_redis.get(record_end));
        return StatusCode::SUCCESS;
    }

    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);

    query.prepare("SELECT end FROM drive_record WHERE id = :drive_record");
    query.bindValue(":drive_record", drive_record);

    if (!query.exec())
    {
        Log::error(QString("%1%2").arg("Query drive record start failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (query.next())
        end = query.value(0).toString();

    if (end.isEmpty())
        return StatusCode::EMPTY_QUERY;

    m_redis.set(record_end, end.toStdString());
    return StatusCode::SUCCESS;
}

QString CTravelRecordDAO::DRIVE_RECORD_START()
{
    static QString start = "DRIVE_RECORD_START";
    return start;
}

QString CTravelRecordDAO::DRIVE_RECORD_END()
{
    static QString end = "DRIVE_RECORD_END";
    return end;
}
