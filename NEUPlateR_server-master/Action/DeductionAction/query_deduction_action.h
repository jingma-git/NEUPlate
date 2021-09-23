#ifndef QUERY_DEDUCTION_ACTION_H
#define QUERY_DEDUCTION_ACTION_H

#include <QJsonArray>
#include "Route/action.h"
#include "Business/Deduction/deduction_controller.h"

/**
* @class         CQueryDeductionAction
* @brief         this calss inherit Action that recive query deduction request from Front End.
* @author        luxijia
* @date          2018-07-31
*/
class CQueryDeductionAction : public CAction
{
public:
    DECLEAR_ACTION(CQueryDeductionAction)
    void run();
};

#endif // QUERY_DEDUCTION_ACTION_H
