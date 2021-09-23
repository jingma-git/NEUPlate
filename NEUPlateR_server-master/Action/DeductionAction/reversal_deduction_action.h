#ifndef REVERSAL_DEDUCTION_ACTION_H
#define REVERSAL_DEDUCTION_ACTION_H

#include <QJsonArray>
#include "Route/action.h"
#include "Business/Deduction/deduction_controller.h"

/**
* @class         CReversalDeductionAction
* @brief         this calss inherit Action that recive reversal deduction request from Front End.
* @author        luxijia
* @date          2018-07-31
*/
class CReversalDeductionAction : public CAction
{
public:
    DECLEAR_ACTION(CReversalDeductionAction)
    void run();
};

#endif // REVERSAL_DEDUCTION_ACTION_H
