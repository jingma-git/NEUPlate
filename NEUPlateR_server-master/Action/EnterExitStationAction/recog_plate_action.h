#ifndef RECOG_PLATE_ACTION_H
#define RECOG_PLATE_ACTION_H

#include "Route/action.h"
#include "Business/RecogRecord/enter_exit_station_controller.h"
#include "Photo/photo_tool.h"
#include "Business/RecogRecord/plate_recog.h"
#include "log/log_handler.h"

class CRecogPlateAction:public CAction
{
public:
    DECLEAR_ACTION(CRecogPlateAction)
    void run();
};

#endif // RECOG_PLATE_ACTION_H
