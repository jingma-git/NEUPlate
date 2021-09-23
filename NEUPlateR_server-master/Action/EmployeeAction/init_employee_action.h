#ifndef INIT_EMPLOYEE_ACTION_H
#define INIT_EMPLOYEE_ACTION_H

#include "Route/action.h"
#include "neu_head.h"

class CInitEmployeeAction : public CAction
{
    DECLEAR_ACTION(CInitEmployeeAction)
public:
    void run();
};

#endif // INIT_EMPLOYEE_ACTION_H
