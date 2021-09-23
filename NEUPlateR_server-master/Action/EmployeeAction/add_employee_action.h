#ifndef ADD_EMPLOYEE_ACTION_H
#define ADD_EMPLOYEE_ACTION_H

#include "Route/action.h"
#include "neu_head.h"

class CAddEmployeeAction : public CAction
{
    DECLEAR_ACTION(CAddEmployeeAction)
public:
    void run();
};

#endif // ADD_EMPLOYEE_ACTION_H
