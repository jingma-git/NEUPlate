#ifndef CUpdateCarAction_H
#define CUpdateCarAction_H
#include "Route/action.h"
#include "Business/Car/car_controller.h"
#include <QJsonArray>
class CQueryCarsAction : public CAction
{
public:
    DECLEAR_ACTION(CQueryCarsAction)
    void run();
};

#endif // CUpdateCarAction_H
