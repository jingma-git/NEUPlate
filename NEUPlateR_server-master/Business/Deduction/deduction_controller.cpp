#include "deduction_controller.h"

/**
* @name          get_deduction
* @brief         get the deduction record by keywaord, time and deduction record state.
* @author        luxijia
* @date          2018-07-31
* @param         records the query result.
* @param         keyword the query condition keyword
* @param         start_time the deduction record start time
* @param         end_time the deduction record end time
* @param         state the deduction record state
* @return        SUCCESS is get deduction record success.
*                QUERY_QUERY is query deduction record error.
*                EMPTY_QUERY is query dedcution record result is empty.
*/
int DeductionController::get_deduction(std::vector<CDedutionRecord> &records, int &all_row_number,
                                       const QString &keyword, const QString &start_time,
                                       const QString &end_time, const int state, const int page_number,
                                       const int page_item)
{
    CDeductionDAO deduction_dao;
    int code;
    int offset = (page_number - 1) * page_item;

    code = deduction_dao.query_deduction(records, all_row_number, keyword, start_time, end_time,
                                         state, offset, page_item);

    if (StatusCode::SUCCESS == code)
    {
        if (0 == records.size())
            return StatusCode::EMPTY_QUERY;
    }
    else if (StatusCode::SQL_EXEC_ERROR == code)
        return StatusCode::QUERY_ERROR;

    return StatusCode::SUCCESS;
}

/**
* @name          delete_deduction
* @brief         delete deduction record by deduction record id.
* @author        luxijia
* @date          2018-07-31
* @param         deduction_id the id of deduction record.
* @return        SUCCESS is delete deduction success.
*                DELETE_ERROR is delete deduction failed.
*/
int DeductionController::delete_deduction(const QString &deduction_id)
{
    CDeductionDAO deduction_dao;
    int code;

    code = deduction_dao.delete_deduction_record(deduction_id);

    if (StatusCode::SQL_EXEC_ERROR == code)
        return StatusCode::DELETE_ERROR;

    return StatusCode::SUCCESS;
}

/**
* @name          is_valied_reversal_deduction
* @brief         judge reversal deduction record whether vailed.
* @author        luxijia
* @date          2018-08-02
* @param         deduction_id the id of deduction
* @return        SUCCESS is query reversal deduction vaildity success.
*                SQL_EXEC_ERROR is exeute sql statement has errors.
*                INVALIED_ERROR is query reversal deduction vaildity failed.
*                TIME_OUT_ERROR is reversal deduction record time out.
*/
int DeductionController::is_valid_reversal_deduction(QString &drive_id, const QString &deduction_id)
{
    CDeductionDAO deduction_dao;
    int code;

    code = deduction_dao.query_reversal_deduction_validity(drive_id, deduction_id);

    if (StatusCode::SUCCESS != code)
         drive_id = "";

    if (StatusCode::SQL_EXEC_ERROR == code)
        return StatusCode::INVAILD_REVERSAL;

    return code;
}

/**
* @name          reversal_deduction
* @brief         insert a new reversal deduction record.
* @author        luxijia
* @date          2018-07-29
* @param         deduction_id the old deduction id.
* @return        REVERSAL_ERROR is reversal deduction record failed.
*                TIME_OUT_ERROR is reversal deduction record time out.
*                SUCCESS is reversal deduction record success.
*/
int DeductionController::reversal_deduction(const QString &deduction_id, const QString &new_deduction_id)
{
    CDeductionDAO deduction_dao;
    int code;

    code = deduction_dao.insert_reversal_deduction_record(deduction_id, new_deduction_id);

    if ((StatusCode::SQL_EXEC_ERROR == code) || (StatusCode::INSERT_ERROR == code))
        return StatusCode::REVERSAL_ERROR;
    else if (StatusCode::TIME_OUT_ERROR == code)
        return code;

    return StatusCode::SUCCESS;
}

/**
* @name          reversal_deduction_by_plate
* @brief         reversal deduction record by plate
* @author        luxijia
* @date          2018-07-29
* @param         deduction_id the old deduction id.
* @return        0 is add reversal deduction record failed.
*                1 is add reversal deduction record success.
*/
int DeductionController::reversal_deduction_by_plate(const QString &deduction_id, const QString &plate)
{
    static bool is_init = false;
    CDeductionDAO deduction_dao;
    QString reversal_deduction_one;
    QString reversal_deduction_two;
    int code;
    CCar car;

    code = Car::query_car_by_plate(car, plate);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Query car by plate failed.");
        return StatusCode::ADD_ERROR;
    }

    if (!is_init)
    {
        is_init = true;
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    }

    reversal_deduction_one = QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + deduction_id.mid(14);
    reversal_deduction_one.chop(2);
    reversal_deduction_two = QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + deduction_id.mid(14);
    reversal_deduction_two.chop(2);

    int rand_one = 10 + qrand() % 90;
    int rand_two;

    while ((rand_two = 10 + qrand() % 90) == rand_one);

    reversal_deduction_one = QString("%1%2").arg(reversal_deduction_one).arg(rand_one);
    reversal_deduction_two = QString("%1%2").arg(reversal_deduction_two).arg(rand_two);

    if (car.payer_id().isEmpty())
        code = deduction_dao.insert_reversal_deduction_record_no_account(deduction_id, reversal_deduction_one, reversal_deduction_two);
    else
        code = deduction_dao.insert_reversal_deduction_record_with_account(deduction_id, reversal_deduction_one,
                                                                           reversal_deduction_two, plate, car.payer_id());

    return code;
}

/**
* @name          auto_deduction
* @brief         automatically calculate highway Deduction and add Deduction record according to drive_record
*                and car type.
* @author        luxijia
* @date          2018-07-29
* @param         payer_id the id of payer
* @param         drive_record the car's drive record in highway.
* @param         car_type the type of car.
* @param         car_situation the situation of car.
* @return        0 if Deduction failed.
*                1 if Deduction success.
*/
double DeductionController::auto_deduction(const QString &payer_id, const QString &drive_record, const QString &car_type, const int car_situation)
{
    CDeductionFactory deduction_factory;
    CDeduction *deduction;
    QString start = TravelRecordController::get_start_toll(drive_record);
    QString end = TravelRecordController::get_end_toll(drive_record);

    QString start_toll = RecogRecordController::queryTollStationId(start);//调用车牌识别模块的查询进口
    QString end_toll = RecogRecordController::queryTollStationId(end); //调用车牌识别模块的查询出口
    QString location;
    int code;
    double total_fee = 0;
    double total_distance = 0;

    std::vector<CHighWay> drive_path = TollController::get_path(start_toll, end_toll);
    int size = drive_path.size();

    if (0 == size)
        return -1;

    for (int i = 0; i < size; i++)
    {
        location = TollController::get_location(drive_path[i].start());
        deduction = deduction_factory.create_deduction(location);
        deduction->set_car_type(car_type);
        total_fee += deduction->deduction(drive_path[i].id(), drive_path[i].distance());
        total_distance += drive_path[i].distance();
    }

    if (FREE_CAR == car_situation)
        total_fee = 0;

    code = TravelRecordController::add_travel_record_distance(drive_record, total_distance);

    if (0 == code)
    {
        Log::error("Add travel record failed.");
        return -1;
    }

    code = add_deduction_record_by_account(payer_id, drive_record, end_toll, PAY_BY_ACCOUNT, total_fee);

    return total_fee;
}

/**
* @name          auto_deduction
* @brief         automatically calculate highway Deduction and add Deduction record according to drive_record
*                and car type.
* @author        luxijia
* @date          2018-07-29
* @param         drive_record the car's drive record in highway.
* @param         car_type the type of car.
* @param         car_situation the situation of car.
* @return        if calculate Deduction success, return positive number
*                if calculate Deduction failed, return negative number
*/
double DeductionController::manual_deduction(const QString &drive_record, const QString &car_type, const int &car_situation)
{
    CDeductionFactory deduction_factory;
    CDeduction *deduction;
    QString start = TravelRecordController::get_start_toll(drive_record);
    QString end = TravelRecordController::get_end_toll(drive_record);

    QString start_toll = RecogRecordController::queryTollStationId(start);//调用车牌识别模块的查询进口
    QString end_toll = RecogRecordController::queryTollStationId(end); //调用车牌识别模块的查询出口
    QString location;
    int code;
    double total_fee = 0;
    double total_distance = 0;

    std::vector<CHighWay> drive_path = TollController::get_path(start_toll, end_toll);
    int size = drive_path.size();

    if (size == 0)
        return -1;

    for (int i = 0; i < size; i++)
    {
        location = TollController::get_location(drive_path[i].start());
        deduction = deduction_factory.create_deduction(location);
        deduction->set_car_type(car_type);
        total_fee += deduction->deduction(drive_path[i].id(), drive_path[i].distance());
        total_distance += drive_path[i].distance();
    }

    if (FREE_CAR == car_situation)
        total_fee = 0;

    code = TravelRecordController::add_travel_record_distance(drive_record, total_distance);

    if (0 == code)
    {
        Log::error("Add travel record failed.");
        return -1;
    }

    if (code == 0)
        return -1;

    return total_fee;
}

/**
* @name          add_deduction_record
* @brief         add a new Deduction record.
* @author        luxijia
* @date          2018-07-29
* @param         drive_record the car's dirve record.
* @param         end_toll the end toll station
* @return        0 is add Deduction record failed.
*                1 is add Deduction record success.
*/
int DeductionController::add_deduction_record(const QString &drive_record, const QString &end_toll, const double fee)
{
    static bool is_init = false;
    CDeductionDAO deduction_dao;
    QDateTime deduction_time = QDateTime::currentDateTime();
    QString plate = "";
    QString start_id = "";


    if (!is_init)
    {
        is_init = true;
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    }

    start_id = TravelRecordController::get_start_toll(drive_record);

    if (start_id.isEmpty())
    {
        Log::error("Get recognition id failed.");
        return StatusCode::INSERT_ERROR;
    }

    plate = RecogRecordController::queryPlateNo(start_id);

    if (plate.isEmpty())
    {
        Log::error("Get recognition plate number failed.");
        return StatusCode::INSERT_ERROR;
    }

    QString deduction_id = QString("%1%2%3").arg(deduction_time.toString("yyyyMMddhhmmss")).arg(end_toll).arg(10 + qrand() % 90);

    return deduction_dao.insert_deduction_record(deduction_id, deduction_time.toString("yyyy-MM-dd hh:mm:ss"),
                                                 drive_record, plate, PAY_BY_MONEY, fee);
}

/**
* @name          add_deduction_record
* @brief         add a new deduction record.
* @author        luxijia
* @date          2018-07-29
* @param         drive_record the car's dirve record.
* @param         end_toll the end toll station
* @return        ADD_ERROR is add deduction record failed.
*                SUCCESS is add deduction record success.
*                FEE_NO_ENOUGH is account is enough.
*/
int DeductionController::add_deduction_record_by_account(const QString &payer_id, const QString &drive_record, const QString &end_toll, const int pay_method, const double fee)
{
    static bool is_init = false;
    int code;
    CDeductionDAO deduction_dao;
    QDateTime deduction_time = QDateTime::currentDateTime();
    QString plate = "";
    QString start_id = "";

    if (!is_init)
    {
        is_init = true;
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    }

    start_id = TravelRecordController::get_start_toll(drive_record);

    if (start_id.isEmpty())
    {
        Log::error("Get recognition id failed.");
        return StatusCode::INSERT_ERROR;
    }

    plate = RecogRecordController::queryPlateNo(start_id);

    if (plate.isEmpty())
    {
        Log::error("Get recognition plate number failed.");
        return StatusCode::INSERT_ERROR;
    }

    QString deduction_id = QString("%1%2%3").arg(deduction_time.toString("yyyyMMddhhmmss")).arg(end_toll).arg(10 + qrand() % 90);

    code = deduction_dao.insert_deduction_record_by_payer(payer_id, deduction_id, deduction_time.toString("yyyy-MM-dd hh:mm:ss"),
                                                          drive_record, plate, pay_method, fee);

    if ((StatusCode::SQL_EXEC_ERROR == code) || (StatusCode::INSERT_ERROR == code))
        return StatusCode::ADD_ERROR;
    else if (StatusCode::FEE_NO_ENOUGH == code)
        return StatusCode::FEE_NO_ENOUGH;

    return StatusCode::SUCCESS;
}

/**
* @name          correct_error_palte
* @brief         correct error plate number.
* @author        luxijia
* @date          2018-08-08
* @param         deduction_id the id of reversal deduction record.
* @param         plate the correct plate number.
* @return        SUCCESS is correct error plate success.
*                CORRECT_PLATE_ERROR is correct error plate failed.
*/
int DeductionController::correct_error_palte(const QString &recogonition_id, const QString &plate)
{
    CDeductionDAO deduction_dao;
    int code;

    code = deduction_dao.insert_correct_error_plate(recogonition_id, plate);

    if (StatusCode::SUCCESS != code)
        return StatusCode::CORRECT_PLATE_ERROR;

    return StatusCode::SUCCESS;
}
