#include "add_employee_action.h"
#include "Business/Employee/employee_controller.h"
#include "Photo/photo_tool.h"

IMPLEMENT_ACTION(add_employee, CAddEmployeeAction)

void CAddEmployeeAction::run()
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
    QString passwd("123456");
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

    QString photo(PhotoTool::save_photo(PhotoTool::from_base64(photo_data)));

    int code = Employee::add_employee(name, passwd, gender, id, photo, access_level, work_place,
                                      birthday, birth_place, married, nation, edu, telephone, email, political_state);

    if(StatusCode::INSERT_ERROR == code){
        resp->set_status_code(StatusCode::SYSTEM_ERROR);
        resp->set_desc("save employee failed");
        return;
    }

    resp->set_status_code(StatusCode::SUCCESS);
    resp->set_desc("save employee successful");
}
