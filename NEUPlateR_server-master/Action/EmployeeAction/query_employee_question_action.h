#ifndef QUERY_EMPLOYEE_QUESTION_ACTION_H
#define QUERY_EMPLOYEE_QUESTION_ACTION_H

#include "Route/action.h"
#include "neu_head.h"

class CQueryEmployeeQuestionAction : public CAction
{
    DECLEAR_ACTION(CQueryEmployeeQuestionAction)
public:
    void run();
};

#endif // QUERY_EMPLOYEE_QUESTION_ACTION_H
