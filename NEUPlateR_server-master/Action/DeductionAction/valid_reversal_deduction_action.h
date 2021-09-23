#ifndef VAILD_REVERSAL_DEDUCTION_ACTION_H
#define VAILD_REVERSAL_DEDUCTION_ACTION_H

#include <QJsonArray>
#include "Photo/photo_tool.h"
#include "Route/action.h"
#include "Business/Deduction/deduction_controller.h"
#include "Business/RecogRecord/recog_record_controller.h"

/**
* @class         CVailedReversalDeductionAction
* @brief         this calss inherit Action that recive is vailed reversal deduction request from Front End.
* @author        luxijia
* @date          2018-07-31
*/
class CVailedReversalDeductionAction : public CAction
{
public:
    DECLEAR_ACTION(CVailedReversalDeductionAction)
    void run();
};

#endif // VAILD_REVERSAL_DEDUCTION_ACTION_H
