#include "query_employees_action.h"
#include "Business/Employee/employee_controller.h"
#include <vector>
#include <QJsonArray>

IMPLEMENT_ACTION(query_employees, CQueryEmployeesAction)

void CQueryEmployeesAction::run()
{
    QString keyword;
    int page;
    int page_num;
    int gender;
    int access_level;
    int state;
    try{
        keyword = req->get_string("keyword");
        page = req->get_int("page");
        page_num = req->get_int("page_num");
        gender = req->get_int("gender");
        access_level = req->get_int("access_level");
        state = req->get_int("state");
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        resp->set_desc("parameters invaild");
        return;
    }

    std::vector<CEmployee> employees;
    int total;
    int code = Employee::query_employees(employees, total, page, page_num, keyword, gender, access_level, state);

    if(StatusCode::EMPTY_QUERY == code){
        resp->set_status_code(StatusCode::NO_SUCH_USER);
        resp->set_desc("no such employee in system");
        return;
    }else if(StatusCode::QUERY_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("query employee error");
        return;
    }

    QJsonArray array;
    for(const auto &e : employees){
        QJsonObject employee;
        employee.insert("username", e.username());
        employee.insert("name", e.name());
        employee.insert("work_place", e.work_place());
        employee.insert("access_level", e.access_level());
        employee.insert("state", e.state());
        array.push_back(employee);
    }

    resp->put("employees", array);
    resp->put("total", total);
    resp->set_status_code(StatusCode::SUCCESS);
    resp->set_desc("query employees successful");
}
