#ifndef CQueryCarByPlateAction_H
#define CQueryCarByPlateAction_H
#include "Route/action.h"
#include "Business/Car/car_controller.h"
class CQueryCarByPlateAction : public CAction
{
public:
    DECLEAR_ACTION(CQueryCarByPlateAction)
    void run();
};

#endif // CPAYER_Action_H
