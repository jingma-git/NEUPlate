#include "modify_employee_action.h"
#include "Business/Employee/employee_controller.h"
#include "Photo/photo_tool.h"

#include <QDate>

IMPLEMENT_ACTION(modify_employee, CModifyEmployeeAction)

void CModifyEmployeeAction::run()
{
    QJsonObject json;
    try{
        json = req->get_json("employee");
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        resp->set_desc("parameters invaild");
        return;
    }

    QString name(json["name"].toString());
    QString username(json["username"].toString());
    QString passwd(json["passwd"].toString());
    QString id(json["id"].toString());
    int gender(json["gender"].toInt());
    QString photo_data(json["photo"].toString());
    QString work_place(json["work_place"].toString());
    QDate birthday(QDate::fromString(json["birthday"].toString()));
    QString birth_place(json["birth_place"].toString());
    QString political_state(json["political_status"].toString());
    bool married(json["married"].toBool());
    QString nation(json["nation"].toString());
    int edu(json["education"].toInt());
    QString telephone(json["telephone"].toString());
    QString email(json["email"].toString());
    int access_level(json["access_level"].toInt());

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

    if(!name.isEmpty())
        employee.set_name(name);

    if(!passwd.isEmpty())
        employee.set_password(passwd);

    if(!id.isEmpty())
        employee.set_id(id);

    if(gender == 1 || gender == 2)
        employee.set_gender(gender);

    // handle photo data
    if(!photo_data.isEmpty()){
        // get old data
        QByteArray old = PhotoTool::load_photo(employee.photo_path());
        QByteArray now = PhotoTool::from_base64(photo_data);
        if(old != now){
            PhotoTool::delete_photo(employee.photo_path());
            employee.set_photo_path(PhotoTool::save_photo(now));
        }
    }

    if(!work_place.isEmpty())
        employee.set_work_place(work_place);

    if(!birthday.isNull())
        employee.set_birthday(birthday);

    if(!birth_place.isEmpty())
        employee.set_birth_place(birth_place);

    employee.set_married(married);

    if(!nation.isEmpty())
        employee.set_nation(nation);

    if(edu >= 0 && edu <=7)
        employee.set_education(edu);

    if(!telephone.isEmpty())
        employee.set_telephone(telephone);

    if(!email.isEmpty())
        employee.set_email(email);

    if(access_level > 0)
        employee.set_access_level(access_level);

    if(!political_state.isEmpty())
        employee.set_political_state(political_state);

    code = Employee::modify_employee(employee);

    if(StatusCode::UPDATE_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("update employee failed");
        return;
    }

    resp->set_status_code(StatusCode::SUCCESS);
    resp->set_desc("modify employee successful");
}
