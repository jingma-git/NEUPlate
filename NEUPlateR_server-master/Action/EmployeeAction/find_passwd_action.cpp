#include "find_passwd_action.h"
#include "Business/Employee/question_controller.h"
#include "Business/Employee/employee_controller.h"
#include "Business/Employee/employee.h"

IMPLEMENT_ACTION(find_passwd, CFindPasswdAction)

void CFindPasswdAction::run()
{
    QString username;
    QString passwd;
    QString q_id;
    QString answer;
    try{
        username =req->get_string("username");
        passwd = req->get_string("passwd");
        q_id = req->get_string("q_id");
        answer = req->get_string("answer");
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        resp->set_desc("parameters invaild");
        return;
    }

    // check answer
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
    }

    CEmployee employee;

    // query employee
    code = Employee::query_employee(username, employee);
    if(StatusCode::QUERY_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("query answer failed");
        return;
    }

    if(StatusCode::EMPTY_QUERY == code){
        resp->set_status_code(StatusCode::NO_SUCH_USER);
        resp->set_desc("cannot query such employee");
        return;
    }

    // modify passwd
    employee.set_password(passwd);
    // save modify
    code = Employee::modify_employee(employee);

    if(StatusCode::UPDATE_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("update employee failed");
        return;
    }

    resp->set_status_code(StatusCode::SUCCESS);
    resp->set_desc("set passwd successful");

}
