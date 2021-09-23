#include "toll_dao.h"

QString CTollDAO::HIGHWAY_TOLL_ID = "HIGHWAY_TOLL_ID";
QString CTollDAO::HIGHWAY_TOLL_LOCATION = "HIGHWAY_TOLL_LOCATION";
QString CTollDAO::HIGHWAY_TOLL_PROVINCE = "HIGHWAY_TOLL_PROVINCE";
QString CTollDAO::HIGHWAY_TOLL_DISTANCE = "HIGHWAY_TOLL_DISTANCE";

/**
* @name          CTollDAO
* @brief         class CTollDAO constructor that get database connection from connection pool.
* @author        luxijia
* @date          2018-07-25
*/
CTollDAO::CTollDAO():
    m_redis("localhost")
{

}

/**
* @name          CTollDAO
* @brief         class CTollDAO constructor that .
* @author        luxijia
* @date          2018-07-26
* @param         db {@QSqlDatebase} object
*/
CTollDAO::CTollDAO(const QSqlDatabase &db):
    m_db(db),
    m_redis("localhost")
{

}

/**
* @name          ~CTollDAO
* @brief         class CTollDAO deconstructor that return database connection from connection pool.
* @author        luxijia
* @date          2018-07-25
*/
CTollDAO::~CTollDAO()
{
    ConnectionPool::close_connection(m_db);
}

/**
* @name          query_highway_id
* @brief         query highway id.
* @author        luxijia
* @date          2018-07-26
* @param         id the id of highway.
* @param         start_toll the start toll station.
* @param         end_toll the end toll station.
* @return        SUCCESS is query highway id success.
*                SQL_EXEC_ERROR is execute sql statement error.
*                EMPTY_QUERY is query highway id no result.
*/
int CTollDAO::query_highway_id(QString &id, const QString &start_toll, const QString &end_toll)
{
    std::string highway_id = HIGHWAY_TOLL_ID.toStdString() + "_" + start_toll.toStdString() + "_" +
                             end_toll.toStdString();

    if (m_redis.exists(highway_id))
    {
        id = QString::fromStdString(m_redis.get(highway_id));
        return StatusCode::SUCCESS;
    }

    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);

    query.prepare("SELECT id FROM highway_info WHERE start = :start AND end = :end");
    query.bindValue(":start", start_toll);
    query.bindValue(":end", end_toll);

    if (!query.exec())
    {
        Log::error(QString("%1%2").arg("Get highway id failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (query.next())
        id = query.value("id").toString();

    if (id.isEmpty())
        return StatusCode::EMPTY_QUERY;

    m_redis.set(highway_id, id.toStdString());
    return StatusCode::SUCCESS;
}

/**
* @name          query_toll_total_number
* @brief         query total number in this city.
* @author        luxijia
* @date          2018-07-26
* @param         total_number the total number of province's toll stations
* @param         province the name of province where toll stay in.
* @return        SUCCESS is get total toll stations' number success.
*                SQL_EXEC_ERROR is execute sql statement error.
*                EMPTY_QUERY is query total number of toll stations in special province failed.
*/
int CTollDAO::query_total_toll(std::vector<QString> &tolls, int &total_number, const QString &province)
{
    std::string highway_province = HIGHWAY_TOLL_PROVINCE.toStdString() + "_" + province.toStdString();

    if (m_redis.exists(highway_province))
    {
        std::vector<std::string> keys;

        m_redis.smembers(highway_province).toVector(keys);
        total_number = keys.size();

        for (int i = 0; i < total_number; i++)
            tolls.push_back(QString::fromStdString(keys[i]));


        return StatusCode::SUCCESS;
    }

    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);

    m_db.transaction();
    query.prepare("SELECT id FROM toll_station_info WHERE province IN (SELECT province FROM name_mapping\
                 WHERE code = :province)");
    query.bindValue(":province", province);

    if (!query.exec())
    {
        m_db.rollback();
        Log::error(QString("%1%2").arg("Get toll stactions' total number failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    while (query.next())
        tolls.push_back(query.value(0).toString());

    total_number = tolls.size();

    if (tolls.empty())
        return StatusCode::EMPTY_QUERY;

    for (int i = 0; i < total_number; i++)
        m_redis.sadd(highway_province, tolls[i].toStdString());

    m_db.commit();
    return StatusCode::SUCCESS;
}

/**
* @name          query_toll_location
* @brief         query toll station location.
* @author        luxijia
* @date          2018-07-25
* @param         id the toll station id.
* @param         location the place location, such as province name or city name.
* @return        SUCCESS is get two place distance success.
*                SQL_EXEC_ERROR is execute sql statement error.
*                EMPTY_QUERY is query two nodistance.
*/
int CTollDAO::query_toll_location(QString &location, const QString &id)
{
    std::string toll_location = HIGHWAY_TOLL_LOCATION.toStdString() + "_" + id.toStdString();

    if (m_redis.exists(toll_location))
    {
        location = QString::fromStdString(m_redis.get(toll_location));
        return StatusCode::SUCCESS;
    }

    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    QSqlQuery query(m_db);

    query.prepare("SELECT name_mapping.code FROM toll_station_info,name_mapping WHERE toll_station_info.id = :id\
                  AND toll_station_info.province = name_mapping.province");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        Log::warning(QString("%1%2").arg("Get toll staction location failed. Error:").arg(query.lastError().text()));
        return StatusCode::SQL_EXEC_ERROR;
    }

    if (!query.next())
        return StatusCode::EMPTY_QUERY;

    location = query.value(0).toString();
    m_redis.set(toll_location, location.toStdString());
    return StatusCode::SUCCESS;
}

/**
* @name          query_toll_distance
* @brief         query two place distance from
* @author        luxijia
* @date          2018-07-25
* @param         distance the tow place distance.
* @param         start_id start place id
* @param         end_id end place id
* @return        SUCCESS is get two place distance success.
*                SQL_EXEC_ERROR is execute sql statement error.
*                QUERY_ERROR is query toll distance error.
*/
int CTollDAO::query_toll_distance(const int total_number, CHighWayMap *highway_map)
{
    bool is_exit = true;
    CHighWayToll *highway_toll = highway_map->toll_array();

    //judge if all date in redis
    for (int i = 0; i <total_number; i++)
    {
        std::string toll_info = HIGHWAY_TOLL_DISTANCE.toStdString() + "_" + highway_toll[i].toll_name().toStdString();

        if (!m_redis.exists(toll_info))
            is_exit = false;
    }

    //if all data in redis then read and return
    if (is_exit)
    {
        for (int i = 0; i < total_number; i++)
        {
            int size;
            std::vector<std::string> keys;
            std::string toll_info = HIGHWAY_TOLL_DISTANCE.toStdString() + "_" + highway_toll[i].toll_name().toStdString();

            m_redis.lrange(toll_info, 0, m_redis.llen(toll_info)).toVector(keys);
            size = keys.size();

            for (int j = 0; j < size; j++)
            {
                QString key = QString::fromStdString(keys[j]);
                CLinkToll *temp = new CLinkToll(key.section(",", 0, 0).toInt(), key.section(",", 1, 1).toDouble());
                temp->setNext(highway_toll[i].head()->next());
                highway_toll[i].head()->setNext(temp);
            }
        }

        return StatusCode::SUCCESS;
    }

    if (!m_db.isOpen())
        m_db = ConnectionPool::open_connection();

    //query data from database
    QSqlQuery query(m_db);

    m_db.transaction();

    for (int i = 0; i < total_number; i++)
    {
        query.prepare("SELECT * FROM highway_info WHERE start = :start");
        query.bindValue(":start", highway_toll[i].toll_name());

        if (!query.exec())
        {
            m_db.rollback();
            Log::error(QString("%1%2").arg("Get toll staction distance failed. Error:").arg(query.lastError().text()));
            return StatusCode::SQL_EXEC_ERROR;
        }

        std::string toll_info = HIGHWAY_TOLL_DISTANCE.toStdString() + "_" + highway_toll[i].toll_name().toStdString();

        while (query.next())
        {
            int index = highway_map->find_toll_index(query.value("end").toString());

            if (-1 == index)
            {
                m_db.rollback();
                Log::error("Toll no in record.");
                return StatusCode::QUERY_ERROR;
            }

            CLinkToll *temp = new CLinkToll(index, query.value("distance").toDouble());
            temp->setNext(highway_toll[i].head()->next());
            highway_toll[i].head()->setNext(temp);

            //wirte to redis
            std::string info = std::to_string(index) + "," + std::to_string(query.value("distance").toDouble());
            m_redis.lpush(toll_info, info);
        }
    }

    m_db.commit();
    return StatusCode::SUCCESS;
}

