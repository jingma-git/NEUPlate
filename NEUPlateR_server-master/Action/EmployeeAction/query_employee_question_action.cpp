#include "query_employee_question_action.h"
#include "Business/Employee/question_controller.h"
#include <map>
#include <QJsonObject>

IMPLEMENT_ACTION(query_employee_question, CQueryEmployeeQuestionAction)

void CQueryEmployeeQuestionAction::run()
{
    QString username;
    try{
        username = req->get_string("username");
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        resp->set_desc("parameters invaild");
        return;
    }

    std::map<QString, QString> questions;
    int code = Question::get_employee_question(username, questions);
    if (StatusCode::EMPTY_QUERY == code){
        resp->set_status_code(StatusCode::NEW_EMPLOYEE);
        resp->set_desc("new employee need init");
        return;
    }else if(StatusCode::QUERY_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("query employee question failed");
        return;
    }
    QJsonObject params;

    for(const auto &q : questions){
        params.insert(q.first, q.second);
    }

    resp->set_status_code(StatusCode::SUCCESS);
    resp->set_desc("query employee question successful");
    resp->put("questions", params);

}
