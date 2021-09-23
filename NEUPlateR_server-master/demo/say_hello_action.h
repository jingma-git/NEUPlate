#ifndef SAY_HELLO_ACTION_H
#define SAY_HELLO_ACTION_H

#include "Route/action.h"

class SayHelloAction : public CAction
{
    DECLEAR_ACTION(SayHelloAction)
public:
    void run();
};

#endif // SAY_HELLO_ACTION_H
