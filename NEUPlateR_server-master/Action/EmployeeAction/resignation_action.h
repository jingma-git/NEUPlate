#ifndef RESIGNATION_ACTION_H
#define RESIGNATION_ACTION_H

#include "Route/action.h"
#include "neu_head.h"

class CResignationAction : public CAction
{
    DECLEAR_ACTION(CResignationAction)
public:
    void run();
};

#endif // RESIGNATION_ACTION_H
