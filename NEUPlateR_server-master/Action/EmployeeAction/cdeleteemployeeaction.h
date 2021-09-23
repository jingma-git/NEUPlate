#ifndef CDELETEEMPLOYEEACTION_H
#define CDELETEEMPLOYEEACTION_H

#include "neu_head.h"
#include "Route/action.h"

class CDeleteEmployeeAction : public CAction
{
    DECLEAR_ACTION(CDeleteEmployeeAction)
public:
    void run();
};

#endif // CDELETEEMPLOYEEACTION_H
