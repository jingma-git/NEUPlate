#ifndef TOLL_STATION_DAO_H
#define TOLL_STATION_DAO_H

#include <QSqlDatabase>
#include <QtGlobal>
#include "toll_station.h"
#include "lane.h"
#include "pool/connection_pool.h"
#include "neu_head.h"
#include "log/log_handler.h"
#include "status_code.h"
#include "RedisClient/redis.h"

class CTollStationDao
{
public:
    CTollStationDao();
    ~CTollStationDao();
    int query_provinces(std::vector<QString> &provinces);
    int query_stations(QString &province, std::vector<CTollStation> &stations);
private:
    QSqlDatabase m_db;

    void add_station_redis(CTollStation &station);
    void get_station_redis(const QString &station_id, CTollStation &station);
};

#endif // TOLL_STATION_DAO_H
