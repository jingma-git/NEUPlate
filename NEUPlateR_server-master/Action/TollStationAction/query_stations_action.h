#ifndef QUERY_STATIONS_ACTION_H
#define QUERY_STATIONS_ACTION_H
#include "Route/action.h"
#include "status_code.h"
#include "Business/TollStation/toll_station_controller.h"
#include "Business/TollStation/toll_station.h"
#include <QJsonArray>

class CQueryStationsAction:public CAction
{
public:
    DECLEAR_ACTION(CQueryStationsAction)
    void run();
};

#endif // QUERY_STATIONS_ACTION_H
