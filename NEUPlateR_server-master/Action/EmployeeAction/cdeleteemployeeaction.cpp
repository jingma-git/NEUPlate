#include "cdeleteemployeeaction.h"
#include "Business/Employee/employee_controller.h"
#include "Business/Employee/question_controller.h"
#include <QJsonArray>

IMPLEMENT_ACTION(delete_employees, CDeleteEmployeeAction)

void CDeleteEmployeeAction::run()
{
    QJsonArray list;
    try{
        list = req->get_array("usernames");
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        resp->set_desc("parameters invaild");
        return;
    }

    std::vector<QString> usernames;
    for(const auto item : list){
        usernames.emplace_back(item.toString());
    }

    int code = Employee::delete_employees(usernames);

    if(StatusCode::DELETE_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("delete employee error");
        return;
    }

    resp->set_status_code(StatusCode::SUCCESS);
    resp->set_desc("delete employees successful");
}
