#ifndef QUERY_EMPLOYEES_ACTION_H
#define QUERY_EMPLOYEES_ACTION_H

#include "neu_head.h"
#include "Route/action.h"

class CQueryEmployeesAction : public CAction
{
    DECLEAR_ACTION(CQueryEmployeesAction)
public:
    void run();
};

#endif // QUERY_EMPLOYEES_ACTION_H
