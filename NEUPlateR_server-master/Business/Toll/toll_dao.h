#ifndef TOLL_DAO_H
#define TOLL_DAO_H

/**
* @name          CTollDAO
* @brief         This class deal with request with database
*                about toll function.
* @author        luxijia
* @date          2018-07-25
*/

#include "RedisClient/hiredispp.h"
#include "neu_head.h"
#include "highway_map.h"
#include "highway_toll.h"
#include "link_toll.h"
#include "pool/connection_pool.h"

class CTollDAO
{
public:
    CTollDAO();
    CTollDAO(const QSqlDatabase &db);
    ~CTollDAO();
    int query_highway_id(QString &id, const QString &start_toll, const QString &end_toll);
    int query_total_toll(std::vector<QString> &tolls, int &total_number, const QString &province);
    int query_toll_location(QString &location, const QString &id);
    int query_toll_distance(const int total_number, CHighWayMap *highway_map);
    int query_toll_information(const QString &id);
    int insert_new_toll_station(const QString &station_name, const QString &province, const QString &city);
    int delete_toll_station(const QString &id);
private:
    static QString HIGHWAY_TOLL_ID;
    static QString HIGHWAY_TOLL_PROVINCE;
    static QString HIGHWAY_TOLL_LOCATION;
    static QString HIGHWAY_TOLL_DISTANCE;
    QSqlDatabase m_db;
    hiredispp::Redis m_redis;
};

#endif // TOLL_DAO_H
