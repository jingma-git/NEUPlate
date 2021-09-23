#include "query_employee_action.h"
#include "Business/Employee/employee_controller.h"

#include <QJsonObject>

IMPLEMENT_ACTION(query_employee, CQueryEmployeeAction)

void CQueryEmployeeAction::run()
{
    QString username;
    try{
        username = req->get_string("username");
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
        resp->set_desc("query employee error");
        return;
    }

    resp->put("employee", employee.to_json());
    resp->set_status_code(StatusCode::SUCCESS);
    resp->set_desc("query employee successful");
}
