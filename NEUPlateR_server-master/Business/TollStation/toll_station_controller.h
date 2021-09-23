#ifndef TOLL_STATION_CONTROLLER_H
#define TOLL_STATION_CONTROLLER_H
/**
* @projectName   TollStationController
* @brief         收费站信息控制
* @author        majing
* @date          2018-07-29
* @modify_author XXXX
* @modify_date   2018-07-29
*/

#include <QString>
#include "toll_station.h"
#include "toll_station_dao.h"
#include "log/log_handler.h"

namespace TollStationController
{
    int query_provinces(std::vector<QString> &provinces);
    int query_stations(QString &province, std::vector<CTollStation> &stations);
}

#endif // TOLL_STATION_CONTROLLER_H
