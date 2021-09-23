/**
* @projectName   toll_station_controller
* @brief         brief
* @author        majing
* @date          2018-07-29
* @modify_author XXXX
* @modify_date   2018-07-29
*/
#include "toll_station_controller.h"

int TollStationController::query_provinces(std::vector<QString> &provinces){
    Log::debug("TollStationController::query_provinces");
    CTollStationDao stationDao;
    return stationDao.query_provinces(provinces);
}

int TollStationController::query_stations(QString &province, std::vector<CTollStation> &stations){
    CTollStationDao stationDao;
    return stationDao.query_stations(province, stations);
}
