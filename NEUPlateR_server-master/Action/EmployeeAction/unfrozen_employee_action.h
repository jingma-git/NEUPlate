#ifndef UNFORZEN_EMPLOYEE_ACTION_H
#define UNFORZEN_EMPLOYEE_ACTION_H

#include "Route/action.h"
#include "neu_head.h"

class CUnfrozenEmployeeAction : public CAction
{
    DECLEAR_ACTION(CUnfrozenEmployeeAction)
public:
    void run();
};

#endif // UNFORZEN_ACTION_H
