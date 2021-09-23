#ifndef SET_QUESTIONS_ACTION_H
#define SET_QUESTIONS_ACTION_H

#include "neu_head.h"
#include "Route/action.h"

class CSetQuestionsAction : public CAction
{
    DECLEAR_ACTION(CSetQuestionsAction)
public:
    void run();
};

#endif // SET_QUESTIONS_ACTION_H
