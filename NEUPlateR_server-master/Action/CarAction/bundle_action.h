#ifndef CBundleCarAction_H
#define CBundleCarAction_H
#include "Route/action.h"
#include "Business/Car/car_controller.h"
class CBundleCarAction : public CAction
{
public:
    DECLEAR_ACTION(CBundleCarAction)
    void run();
};

#endif // CPAYER_Action_H
