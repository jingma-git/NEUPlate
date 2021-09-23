#ifndef PAY_BY_CASH_ACTION_H
#define PAY_BY_CASH_ACTION_H

#include "Route/action.h"
#include "Business/Deduction/deduction_controller.h"
class CPayByCashAction:public CAction
{
public:
    DECLEAR_ACTION(CPayByCashAction)
    void run();
};

#endif // PAY_BY_CASH_ACTION_H
