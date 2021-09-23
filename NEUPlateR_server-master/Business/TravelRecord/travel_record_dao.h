#ifndef DRIVE_RECORD_DAO_H
#define DRIVE_RECORD_DAO_H

#include "RedisClient/hiredispp.h"
#include "neu_head.h"
#include "pool/connection_pool.h"


/**
* @class         CTravelRecordDAO
* @brief         This class deal with request with database
*                about toll function.
* @author        luxijia
* @date          2018-07-27
*/

class CTravelRecordDAO
{
public:
    CTravelRecordDAO();
    CTravelRecordDAO(const QSqlDatabase &db);
    ~CTravelRecordDAO();
    int insert_start_place(const QString &start);
    int insert_end_place(QString &record_id, const QString &end, const QString &plate);
    int update_drive_distance(const QString &record_id, const double distance);
    int query_drive_record_start(QString &start, const QString &drive_record);
    int query_drive_record_end(QString &end, const QString &drive_record);
private:
    static QString DRIVE_RECORD_START();
    static QString DRIVE_RECORD_END();
    QSqlDatabase m_db;
    hiredispp::Redis m_redis;
};

#endif // DRIVE_RECORD_DAO_H
