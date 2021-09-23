#ifndef DELETE_DEDUCTION_ACTION_H
#define DELETE_DEDUCTION_ACTION_H

#include <QJsonArray>
#include "Route/action.h"
#include "Business/Deduction/deduction_controller.h"

/**
* @class         CDeleteDeductionAction
* @brief         this calss inherit Action that recive delete deduction request from Front End.
* @author        luxijia
* @date          2018-07-31
*/
class CDeleteDeductionAction : public CAction
{
public:
    DECLEAR_ACTION(CDeleteDeductionAction)
    void run();
};

#endif // DELETE_DEDUCTION_ACTION_H
