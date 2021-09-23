#ifndef FIND_PASSWD_ACTION_H
#define FIND_PASSWD_ACTION_H

#include "neu_head.h"
#include "Route/action.h"

class CFindPasswdAction : public CAction
{
    DECLEAR_ACTION(CFindPasswdAction)
public:
    void run();
};

#endif // FIND_PASSWD_ACTION_H
