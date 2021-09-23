#ifndef CPAYER_Action_H
#define CPAYER_Action_H
#include "Route/action.h"
#include "Business/Payer/payer_controller.h"
class CValidPayerAction : public CAction
{
public:
    DECLEAR_ACTION(CValidPayerAction)
    void run();
};

#endif // CPAYER_Action_H
