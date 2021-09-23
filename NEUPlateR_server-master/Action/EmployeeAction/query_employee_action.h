#ifndef QUERY_EMPLOYEE_ACTION_H
#define QUERY_EMPLOYEE_ACTION_H

#include "Route/action.h"
#include "neu_head.h"

class CQueryEmployeeAction : public	CAction
{
    DECLEAR_ACTION(CQueryEmployeeAction)
public:
    void run();
};

#endif // QUERY_EMPLOYEE_ACTION_H
