#include "resignation_action.h"
#include "Business/Employee/employee_controller.h"
#include <QJsonArray>

IMPLEMENT_ACTION(resignation, CResignationAction)

void CResignationAction::run()
{
    QJsonArray username;
    try{
        username = req->get_array("usernames");
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        resp->set_desc("parameters invaild");
        return;
    }

    int code = 0;
    for(const auto &u : username){
        code = Employee::add_state(u.toString(), EmployeeState::RESIGNATION);
        if(StatusCode::EMPTY_QUERY == code){
            resp->set_status_code(StatusCode::NO_SUCH_USER);
            resp->set_desc("no such employee in system");
            return;
        }else if(StatusCode::UPDATE_ERROR == code){
            resp->set_status_code(StatusCode::SYSTEM_ERROR);
            resp->set_desc("leave employee error");
            return;
        }
    }

    resp->set_status_code(StatusCode::SUCCESS);
    resp->set_desc("leave employees successful");
}
