#ifndef MODIFY_EMPLOYEE_ACTION_H
#define MODIFY_EMPLOYEE_ACTION_H

#include "Route/action.h"
#include "neu_head.h"

class CModifyEmployeeAction : public CAction
{
    DECLEAR_ACTION(CModifyEmployeeAction)
public:
    void run();
};

#endif // MODIFY_EMPLOYEE_ACTION_H
