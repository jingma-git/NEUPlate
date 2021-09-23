#include "query_questions_action.h"
#include "Business/Employee/question_controller.h"

#include <map>
#include <QJsonObject>

IMPLEMENT_ACTION(query_questions, CQueryQuestionsAction)

void CQueryQuestionsAction::run()
{
    std::map<QString, QString> id_question;
    QJsonObject questions;

    int code = Question::get_security_questions(id_question);

    if(StatusCode::QUERY_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("query security question failed");
    }

    for(const auto &q : id_question){
        questions.insert(q.first, q.second);
    }

    resp->set_status_code(StatusCode::SUCCESS);
    resp->set_desc("query security question successful");
    resp->put("questions", questions);

}
