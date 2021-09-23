#include "check_question_action.h"
#include "Business/Employee/question_controller.h"

IMPLEMENT_ACTION(check_question, CCheckQuestionAction)

void CCheckQuestionAction::run()
{
    QString username;
    QString q_id;
    QString answer;
    try{
        username = req->get_string("username");
        q_id = req->get_string("q_id");
        answer = req->get_string("answer");
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        resp->set_desc("parameters invaild");
        return;
    }

    int code = Question::check_employee_answer(username, q_id, answer);

    if(StatusCode::QUERY_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("query answer failed");
        return;
    }

    if(StatusCode::EMPTY_QUERY == code){
        resp->set_status_code(StatusCode::EMPTY_QUERY);
        resp->set_desc("cannot query such question and answer");
        return;
    }

    if(StatusCode::WRONG_ANSWER == code){
        resp->set_status_code(StatusCode::WRONG_ANSWER);
        resp->set_desc("the question answer is wrong");
        return;
    }

    resp->set_status_code(StatusCode::SUCCESS);
    resp->set_desc("check employee answer successful");
}
