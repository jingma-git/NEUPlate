#ifndef CPAYER_Action_H
#define CPAYER_Action_H
#include "Route/action.h"
#include "Business/Payer/payer_controller.h"
#include <QJsonArray>
class CQueryPayersAction : public CAction
{
public:
    DECLEAR_ACTION(CQueryPayersAction)
    void run();
};

#endif // CPAYER_Action_H
