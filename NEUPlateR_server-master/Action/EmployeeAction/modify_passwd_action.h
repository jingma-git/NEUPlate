#ifndef MODIFY_PASSWD_ACTION_H
#define MODIFY_PASSWD_ACTION_H

#include "Route/action.h"
#include "neu_head.h"

class CModifyPasswdAction : public CAction
{
    DECLEAR_ACTION(CModifyPasswdAction)
public:
    void run();
};

#endif // MODIFY_PASSWD_ACTION_H
