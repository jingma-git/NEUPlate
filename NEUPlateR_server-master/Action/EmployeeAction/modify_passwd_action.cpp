#include "modify_passwd_action.h"
#include "Business/Employee/employee_controller.h"


IMPLEMENT_ACTION(modify_passwd, CModifyPasswdAction)

void CModifyPasswdAction::run()
{
    QString username;
    QString old_passwd;
    QString new_passwd;
    try{
        username = req->get_string("username");
        old_passwd = req->get_string("old_passwd");
        new_passwd = req->get_string("new_passwd");
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        resp->set_desc("parameters invaild");
        return;
    }

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

    if(employee.password() != old_passwd){
        resp->set_status_code(StatusCode::INVAILD_PASSWD);
        resp->set_desc("old password error");
        return;
    }

    employee.set_password(new_passwd);

    code = Employee::modify_employee(employee);

    if(StatusCode::UPDATE_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("update employee failed");
        return;
    }

    resp->set_status_code(StatusCode::SUCCESS);
    resp->set_desc("modify password successful");
}
