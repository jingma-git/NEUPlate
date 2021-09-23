#ifndef CDeleteCarAction_H
#define CDeleteCarAction_H
#include "Route/action.h"
#include "Business/Car/car_controller.h"
class CDeleteCarAction : public CAction
{
public:
    DECLEAR_ACTION(CDeleteCarAction)
    void run();
};

#endif // CDeleteCarAction_H
