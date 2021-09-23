#include "login_action.h"
#include "Exception/null_exception.h"
#include "status_code.h"
#include "Business/Employee/employee.h"
#include "Business/Employee/employee_controller.h"

IMPLEMENT_ACTION(login, CLoginAction)

/**
* @functionName  run()
* @brief         login into system
* @author        chenhanlin
* @date          2018-07-30
* @parameter     void
* @return        void
*/
void CLoginAction::run()
{
    QString username;
    QString passwd;
    try{
        username = req->get_string("username");
        passwd = req->get_string("passwd");
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        resp->set_desc("parameters invaild");
        return;
    }

    if(Employee::check_login(username)){
        resp->set_status_code(StatusCode::LOGINED);
        resp->set_desc("employee already logined");
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

    int now_state = employee.state();

    if(now_state & EmployeeState::RESIGNATION){
        resp->set_status_code(StatusCode::RESIGNATION);
        resp->set_desc("employee is resignation");
        return;
    }

    if(now_state & EmployeeState::FROZEN){
        resp->set_status_code(StatusCode::FROZEN);
        resp->set_desc("employee's account is frozen");
        return;
    }


    if(now_state & EmployeeState::NEW_EMPLOYEE){
        resp->set_status_code(StatusCode::NEW_EMPLOYEE);
        resp->set_desc("employee need init");
        return;
    }

    if(passwd == employee.password()){
        resp->set_status_code(StatusCode::SUCCESS);
        resp->set_desc("employee login into system");
        Employee::log_login(username, m_socket_id, employee.access_level());
        resp->put("access_level", employee.access_level());
        return;
    }else{
        resp->set_status_code(StatusCode::INVAILD_PASSWD);
        resp->set_desc("password invaild");
        Employee::log_login_error_times(username);
        return;
    }
}
