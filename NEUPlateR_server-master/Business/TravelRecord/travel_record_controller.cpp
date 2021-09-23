#include "travel_record_controller.h"

/**
* @name          add_travel_record_distance
* @brief         add travel record total distance
* @author        luxijia
* @date          2018-07-31
* @param         drive_record drive record id.
* @param         distance the total distance of drive record.
* @return        1 is add success.
*                0 is add failed.
*/
int TravelRecordController::add_travel_record_distance(const QString &drive_reord, const double distance)
{
    CTravelRecordDAO drive_dao;

    int code;

    code = drive_dao.update_drive_distance(drive_reord, distance);

    if (StatusCode::SUCCESS == code)
        return 1;

    return 0;
}

/**
* @name          add_start_toll
* @brief         record drive start information.
* @author        luxijia
* @date          2018-07-27
* @param         start the start place id
* @return        1 is add success
*                0 is add failed.
*/
int TravelRecordController::add_start_toll(const QString &start)
{
    //qDebug()<<"add_start_toll";
    CTravelRecordDAO drive_dao;
    int code;

    code = drive_dao.insert_start_place(start);

    if (StatusCode::SUCCESS == code)
        return 1;
    else
        return 0;
}

/**
* @name          add_end_toll
* @brief         record drive end information.
* @author        luxijia
* @date          2018-07-27
* @param         start the start place id
* @param         end the end place id
* @return        non-empty string is add success
*                empty string is add failed.
*/
QString TravelRecordController::add_end_toll(const QString &end, const QString &plate)
{
    CTravelRecordDAO drive_dao;
    QString record_id;
    int code;

    code = drive_dao.insert_end_place(record_id, end, plate);


    if (StatusCode::SUCCESS == code)
        return record_id;
    else
        return "";
}

/**
* @name          get_start_toll
* @brief         Get start toll station according to drive record id.
* @author        luxijia
* @date          2018-07-27
* @param         drive_record drive record id.
* @return        non-empty string is add success
*                empty string is add failed.
*/
QString TravelRecordController::get_start_toll(const QString &drive_record)
{
    CTravelRecordDAO drive_dao;
    QString start = "";
    int code;

    code = drive_dao.query_drive_record_start(start, drive_record);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get start place failed.");
        return "";
    }

    return start;
}

/**
* @name          get_end_toll
* @brief         Get end toll station according to drive record id.
* @author        luxijia
* @date          2018-07-27
* @param         drive_record drive record id.
* @return        non-empty string is end success
*                empty string is end failed.
*/
QString TravelRecordController::get_end_toll(const QString &drive_record)
{
    CTravelRecordDAO drive_dao;
    QString end = "";
    int code;

    code = drive_dao.query_drive_record_end(end, drive_record);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get end place failed.");
        return "";
    }

    return end;
}
