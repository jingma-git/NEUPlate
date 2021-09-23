#ifndef EXIT_STATION_ACTION_H
#define EXIT_STATION_ACTION_H

#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>

#include "Route/action.h"
#include "Photo/photo_tool.h"
#include "Business/RecogRecord/recog_record_controller.h"
#include "Business/RecogRecord/recog_record.h"
#include "Business/Car/car_controller.h"
#include "Business/Car/car.h"
#include "Business/Deduction/deduction_controller.h"
#include "Business/TravelRecord/travel_record_controller.h"
#include "log/log_handler.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <ctime>


class CExitStationAction:public CAction
{
public:
    DECLEAR_ACTION(CExitStationAction)
    void run();
};

#endif // EXIT_STATION_ACTION_H
