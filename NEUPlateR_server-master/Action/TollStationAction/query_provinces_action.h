#ifndef QUERY_PROVINCES_ACTION_H
#define QUERY_PROVINCES_ACTION_H
#include "Route/action.h"
#include "Business/TollStation/toll_station_controller.h"
#include "Business/TollStation/toll_station.h"
#include <QJsonArray>

class CQueryProvincesAction:public CAction
{
public:
    DECLEAR_ACTION(CQueryProvincesAction)
    void run();
};

#endif // QUERY_PROVINCES_ACTION_H
