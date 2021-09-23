#ifndef QUERY_QUESTIONS_ACTION_H
#define QUERY_QUESTIONS_ACTION_H

#include "Route/action.h"
#include "neu_head.h"

class CQueryQuestionsAction : public CAction
{
    DECLEAR_ACTION(CQueryQuestionsAction)
public:
    void run();
};

#endif // QUERY_QUESTIONS_ACTION_H
