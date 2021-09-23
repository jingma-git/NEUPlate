#ifndef CUpdateCarAction_H
#define CUpdateCarAction_H
#include "Route/action.h"
#include "Business/Car/car_controller.h"
class CUpdateCarAction : public CAction
{
public:
    DECLEAR_ACTION(CUpdateCarAction)
    void run();
};

#endif // CUpdateCarAction_H
