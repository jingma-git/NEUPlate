#include "employee.h"
#include "Photo/photo_tool.h"
#include <QJsonObject>

CEmployee::CEmployee()
{

}

QString CEmployee::name() const
{
    return m_name;
}

void CEmployee::set_name(const QString &name)
{
    m_name = name;
}

QString CEmployee::username() const
{
    return m_username;
}

void CEmployee::set_username(const QString &username)
{
    m_username = username;
}

QString CEmployee::password() const
{
    return m_password;
}

void CEmployee::set_password(const QString &password)
{
    m_password = password;
}

QString CEmployee::id() const
{
    return m_id;
}

void CEmployee::set_id(const QString &id)
{
    m_id = id;
}

int CEmployee::gender() const
{
    return m_gender;
}

void CEmployee::set_gender(int gender)
{
    m_gender = gender;
}

QString CEmployee::photo_path() const
{
    return m_photo_path;
}

void CEmployee::set_photo_path(const QString &photo_path)
{
    m_photo_path = photo_path;
}

QString CEmployee::work_place() const
{
    return m_work_place;
}

void CEmployee::set_work_place(const QString &work_place)
{
    m_work_place = work_place;
}

QString CEmployee::birthday() const
{
    return m_birthday.toString();
}

void CEmployee::set_birthday(const QDate &birthday)
{
    m_birthday = birthday;
}

QString CEmployee::birth_place() const
{
    return m_birth_place;
}

void CEmployee::set_birth_place(const QString &birth_place)
{
    m_birth_place = birth_place;
}

QString CEmployee::political_state() const
{
    return m_political_state;
}

void CEmployee::set_political_state(const QString &political_state)
{
    m_political_state = political_state;
}

bool CEmployee::married() const
{
    return m_married;
}

void CEmployee::set_married(bool married)
{
    m_married = married;
}

QString CEmployee::nation() const
{
    return m_nation;
}

void CEmployee::set_nation(const QString &nation)
{
    m_nation = nation;
}

int CEmployee::education() const
{
    return m_edu;
}

void CEmployee::set_education(int edu)
{
    m_edu = edu;
}

QString CEmployee::telephone() const
{
    return m_telephone;
}

void CEmployee::set_telephone(const QString &telephone)
{
    m_telephone = telephone;
}

QString CEmployee::email() const
{
    return m_email;
}

void CEmployee::set_email(const QString &email)
{
    m_email = email;
}

int CEmployee::state() const
{
    return m_state;
}

void CEmployee::set_state(int state)
{
    m_state = state;
}

bool CEmployee::is_new() const
{
    return m_is_new;
}

void CEmployee::set_is_new(bool is_new)
{
    m_is_new = is_new;
}

int CEmployee::access_level() const
{
    return m_access_level;
}

void CEmployee::set_access_level(int access_level)
{
    m_access_level = access_level;
}

/**
* @functionName  to_json
* @brief         change entity to json object
* @author        chenhanlin
* @date          2018-07-31
* @parameter     void
* @return        QJsonObejct
*/
QJsonObject CEmployee::to_json()
{
    QJsonObject employee;
    employee.insert("name", m_name);
    employee.insert("work_place", m_work_place);
    employee.insert("gender", m_gender);
    employee.insert("username", m_username);
    employee.insert("id", m_id);
    employee.insert("birthday", m_birthday.toString());
    employee.insert("birth_place", m_birth_place);
    employee.insert("political_state", m_political_state);
    employee.insert("married", m_married);
    employee.insert("nation", m_nation);
    employee.insert("edu", m_edu);
    employee.insert("telephone", m_telephone);
    employee.insert("email", m_email);
    employee.insert("state", m_state);
    employee.insert("access_level", m_access_level);

    // load photo
    employee.insert("photo", PhotoTool::to_base64(PhotoTool::load_photo(m_photo_path)));

    return employee;
}
