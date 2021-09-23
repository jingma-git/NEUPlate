#include "init_employee_action.h"
#include "Business/Employee/employee.h"
#include "Business/Employee/employee_controller.h"
#include "Business/Employee/question_controller.h"

#include <map>
#include <QJsonObject>

IMPLEMENT_ACTION(init_employee, CInitEmployeeAction)

void CInitEmployeeAction::run()
{
    QString username;
    QString passwd;
    QJsonObject questions;
    try{
        username = req->get_string("username");
        passwd = req->get_string("passwd");
        questions = req->get_json("questions");
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        resp->set_desc("parameters invaild");
        return;
    }

    // query employee information
    CEmployee employee;
    int code = Employee::query_employee(username, employee);

    if(StatusCode::EMPTY_QUERY == code){
        resp->set_status_code(StatusCode::NO_SUCH_USER);
        resp->set_desc("no such employee in system");
        return;
    }else if(StatusCode::QUERY_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("init employee error");
        return;
    }

    // check employee is a new employee
    if(!(EmployeeState::NEW_EMPLOYEE & employee.state())){
        resp->set_status_code(StatusCode::NO_NEW_EMPLOYEE);
        resp->set_desc("not a new employee");
        return;
    }

    std::map<QString, QString> id_question;

    for(const auto &key : questions.keys()){
        id_question.insert(std::map<QString, QString>::value_type(key, questions[key].toString()));
    }

    // save security question
    code = Question::set_security_question(username, id_question);

    if(StatusCode::INSERT_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("init employee error");
        return;
    }

    // save new passwd
    employee.set_password(passwd);
    code = Employee::modify_employee(employee);

    if(StatusCode::UPDATE_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("update employee failed");
        return;
    }

    code = Employee::remove_state(username, EmployeeState::NEW_EMPLOYEE);

    if(StatusCode::EMPTY_QUERY == code){
        resp->set_status_code(StatusCode::NO_SUCH_USER);
        resp->set_desc("cannot query such employee");
        return;
    }

    if(StatusCode::UPDATE_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("init employee failed");
        return;
    }

    resp->set_status_code(StatusCode::SUCCESS);
    resp->set_desc("init employee successful");
}
