#ifndef CHECK_QUESTION_ACTION_H
#define CHECK_QUESTION_ACTION_H

#include "neu_head.h"
#include "Route/action.h"

class CCheckQuestionAction : public CAction
{
    DECLEAR_ACTION(CCheckQuestionAction)
public:
    void run();
};

#endif // CHECK_QUESTION_ACTION_H
