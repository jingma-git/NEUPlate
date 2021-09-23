#ifndef LOGIN_ACTION_H
#define LOGIN_ACTION_H

#include "Route/action.h"

class CLoginAction : public CAction
{
    DECLEAR_ACTION(CLoginAction)
public:
    void run();
};

#endif // LOGIN_ACTION_H
