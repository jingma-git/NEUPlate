#ifndef FROZEN_ACTION_H
#define FROZEN_ACTION_H

#include "neu_head.h"
#include "Route/action.h"

class CFrozenAction : public CAction
{
    DECLEAR_ACTION(CFrozenAction)
public:
    void run();
};

#endif // FROZEN_ACTION_H
