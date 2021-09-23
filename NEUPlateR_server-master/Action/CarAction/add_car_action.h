#ifndef CAddCarAction_H
#define CAddCarAction_H
#include "Route/action.h"
#include "Business/Car/car_controller.h"
class CAddCarAction : public CAction
{
public:
    DECLEAR_ACTION(CAddCarAction)
    void run();
};

#endif // CPAYER_Action_H
