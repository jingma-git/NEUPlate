#ifndef UNFROZEN_ACTION_H
#define UNFROZEN_ACTION_H

#include "neu_head.h"
#include "Route/action.h"

class CUnfrozenAction : public CAction
{
    DECLEAR_ACTION(CUnfrozenAction)
public:
    void run();
};

#endif // UNFROZEN_ACTION_H
