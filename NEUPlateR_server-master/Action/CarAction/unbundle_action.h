#ifndef CUnbunleCarAction_H
#define CUnbunleCarAction_H
#include "Route/action.h"
#include "Business/Car/car_controller.h"
class CUnbunleCarAction : public CAction
{
public:
    DECLEAR_ACTION(CUnbunleCarAction)
    void run();
};

#endif // CUnbunleCarAction_H
