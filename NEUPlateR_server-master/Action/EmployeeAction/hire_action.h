#ifndef HIRE_ACTION_H
#define HIRE_ACTION_H

#include "Route/action.h"
#include "neu_head.h"

class CHireAction : public CAction
{
    DECLEAR_ACTION(CHireAction)
public:
    void run();
};

#endif // HIRE_ACTION_H
