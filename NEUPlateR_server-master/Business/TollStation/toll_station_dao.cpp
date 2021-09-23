/**
* @projectName   CTollStationDao
* @brief         brief
* @author        majing
* @date          2018-07-29
* @modify_author XXXX
* @modify_date   2018-07-29
*/
#include "toll_station_dao.h"

CTollStationDao::CTollStationDao():m_db(ConnectionPool::open_connection())
{
}


CTollStationDao::~CTollStationDao(){
    ConnectionPool::close_connection(m_db);
}

/**
* @functionName  query_provinces
* @Description   query all the provinces stored in the database
* @author        majing
* @date          2018-07-29
* @parameter     void
* @return        void
*/

int CTollStationDao::query_provinces(std::vector<QString> &provinces){
    QSqlQuery query(m_db);
    query.prepare("SELECT distinct(province) FROM toll_station_info");
    if(!query.exec()){
        return StatusCode::SQL_EXEC_ERROR;
    }

    while(query.next()){
        QString province = query.value("province").toString();
        provinces.push_back(province);
    }
    return StatusCode::SUCCESS;
}

/**
* @functionName  query_stations
* @Description   query all the station of a specific province
* @author        majing
* @date          2018-07-29
* @parameter     void
* @return        void
*/

int CTollStationDao::query_stations(QString &province, std::vector<CTollStation> &stations){

    Log::debug("CTollStationDao::query_stations");

    Redis redis;
    std::string pro_str = province.toStdString();
    const char* pro_cstr  = pro_str.c_str();

    if(redis.existsKey(pro_cstr)){
        std::vector<std::string> station_ids;
        redis.smembers(pro_str, station_ids);
        for(const std::string &station_id:station_ids){
            CTollStation station;
            QString station_qstr = QString::fromStdString(station_id);
            get_station_redis(station_qstr, station);
            stations.push_back(station);
        }
        return StatusCode::SUCCESS;
    }


    QSqlQuery query(m_db);
    std::vector<QString> station_ids;
    std::vector<QString> station_names;
    query.prepare("SELECT * FROM toll_station_info WHERE province=?");
    query.addBindValue(province);
    if(!query.exec()){
        return StatusCode::SQL_EXEC_ERROR;
    }

    while(query.next()){
        QString id = query.value("id").toString();
        QString name = query.value("name").toString();
        station_ids.push_back(id);
        station_names.push_back(name);
    }
    query.prepare("SELECT * FROM lane_info WHERE station_id=? ORDER BY lane_no ASC");
    int size = station_ids.size();
    for(int i=0; i<size; ++i){
        QString &id = station_ids[i];
        QString &name = station_names[i];

        query.addBindValue(id);
        if(!query.exec()){
            return StatusCode::SQL_EXEC_ERROR;
        }

        std::vector<CLane> lanes;
        int idx= 1;
        while(query.next()){
           int lane_no = query.value("lane_no").toInt();
           bool is_etc = query.value("is_etc").toBool();
           CLane lane(lane_no, is_etc);
           lanes.push_back(lane);
           ++idx;
        }
        CTollStation station(id,name,province,lanes);
        stations.push_back(station);
    }
    return StatusCode::SUCCESS;
}



void CTollStationDao::add_station_redis(CTollStation &station){
    Redis redis;
    std::string station_id= station.id().toStdString();
    std::string pro_name = station.province().toStdString();
    std::string key_str = "station_"+station_id;
    const char *key = key_str.c_str();

    redis.hset(key, "station_id", station_id.c_str());
    redis.hset(key, "name",station.name().toStdString().c_str());
    redis.hset(key,"province",pro_name.c_str());

    redis.sadd(pro_name.c_str(), station_id.c_str());
}

void CTollStationDao::get_station_redis(const QString &station_id, CTollStation& station){
    Redis redis;
    std::string station_id_str= station.id().toStdString();
    std::string key_str = "station_"+station_id_str;
    const char *key = key_str.c_str();
    std::string name = redis.hget(key, "name");
    std::string province = redis.hget(key, "province");


    station.setId(station_id);
    station.setName(QString::fromStdString(name));
    station.setProvince(QString::fromStdString(province));

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM lane_info WHERE station_id=? ORDER BY lane_no ASC");
    std::vector<CLane> lanes;
    while(query.next()){
       int lane_no = query.value("lane_no").toInt();
       bool is_etc = query.value("is_etc").toBool();
       CLane lane(lane_no, is_etc);
       lanes.push_back(lane);
    }
    station.setLanes(lanes);
}
