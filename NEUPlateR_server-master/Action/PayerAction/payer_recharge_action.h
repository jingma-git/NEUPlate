#ifndef CPAYER_Action_H
#define CPAYER_Action_H
#include "Route/action.h"
#include "Business/Payer/payer_controller.h"
class CRechargePayerAction : public CAction
{
public:
    DECLEAR_ACTION(CRechargePayerAction)
    void run();
};

#endif // CPAYER_Action_H
