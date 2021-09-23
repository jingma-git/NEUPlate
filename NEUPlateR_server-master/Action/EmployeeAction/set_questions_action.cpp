#include "set_questions_action.h"
#include "Business/Employee/question_controller.h"

IMPLEMENT_ACTION(set_employee_questions, CSetQuestionsAction)

void CSetQuestionsAction::run()
{
    QString username;
    QJsonObject json;
    try{
        username = req->get_string("username");
        json = req->get_json("questions");
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        resp->set_desc("parameters invaild");
        return;
    }

    std::map<QString, QString> questions;
    for(const auto &key : json.keys())
        questions.insert(std::map<QString, QString>::value_type(key, json[key].toString()));

    int code = Question::set_security_question(username, questions);

    if(StatusCode::INSERT_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("save security questions failed");
        return;
    }

    resp->set_status_code(StatusCode::SUCCESS);
    resp->set_desc("save question successful");
}
