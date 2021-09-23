#include "employee_controller.h"
#include "employee.h"
#include "employee_dao.h"
#include "neu_head.h"
#include "pool/connection_pool.h"

#include <QSettings>

/**
* @functionName  add_employee
* @brief         add a employee into system
* @author        chenhanlin
* @date          2018-07-30
* @parameter     void
* @return        void
*/
int Employee::add_employee(const QString &name, const QString &passwd, const int gender,
                           const QString &id_card, const QString &photo,
                           const int access_level, const QString &work_place, const QDate &birthday,
                           const QString &birth_place, const bool married, const QString &nation,
                           const int edu, const QString &telephone, const QString &email,
                           const QString &political_state)
{
    CEmployeeDAO dao;
    CEmployee employee;

    employee.set_name(name);
    employee.set_password(passwd);
    employee.set_gender(gender);
    employee.set_id(id_card);
    employee.set_state(EmployeeState::NEW_EMPLOYEE | EmployeeState::NORMAL);
    employee.set_photo_path(photo);
    employee.set_access_level(access_level);
    employee.set_work_place(work_place);
    employee.set_birthday(birthday);
    employee.set_birth_place(birth_place);
    employee.set_married(married);
    employee.set_nation(nation);
    employee.set_education(edu);
    employee.set_telephone(telephone);
    employee.set_email(email);
    employee.set_political_state(political_state);
    employee.set_is_new(true);

    int code = dao.save_employee(employee);
    if(code != StatusCode::SUCCESS)
        return StatusCode::INSERT_ERROR;
    return StatusCode::SUCCESS;
}

/**
* @functionName  delete_employee
* @brief         delete employees by their login id
* @author        chenhanlin
* @date          2018-07-30
* @parameter     const std::vector<QString> &username login id set
* @return        SUCCESS delete successful
* 				 DELETE_ERROR delete failed
*/
int Employee::delete_employees(const std::vector<QString> &usernames)
{
    QSqlDatabase db_conn = ConnectionPool::open_connection();
    CEmployeeDAO dao(db_conn);
    int code = 0;
    db_conn.transaction();

    for(const auto &key : usernames){
        code = dao.delete_employee(key);
        if(StatusCode::SUCCESS != code){
            db_conn.rollback();
            return StatusCode::DELETE_ERROR;
        }
    }
    db_conn.commit();
    return StatusCode::SUCCESS;
}

/**
* @functionName  query_employee
* @brief         get a employee's information by his login id
* @author        chenhanlin
* @date          2018-07-30
* @parameter     const QString &username login id
* 				 CEmloyee &employee output parameter Employee entity
* @return        SUCCESS query successful
* 				 QUERY_ERROR query failed
* 				 EMPTY_QUERY no such employee
*/
int Employee::query_employee(const QString &username, CEmployee &employee)
{
    CEmployeeDAO dao;
    int code = dao.query_employee(username, employee);

    if(StatusCode::SQL_EXEC_ERROR == code){
        return StatusCode::QUERY_ERROR;
    }

    return code;
}

/**
* @functionName  modify_employee
* @brief         modify_employee's information
* @author        chenhanlin
* @date          2018-07-30
* @parameter     const CEmployee &employee employee entity
* @return        SUCCESS modify success
* 				 UPDATE_ERROR modify failed
*/
int Employee::modify_employee(const CEmployee &employee)
{
    CEmployeeDAO dao;
    int code = dao.save_employee(employee);
    if(StatusCode::SQL_EXEC_ERROR == code)
        return StatusCode::UPDATE_ERROR;

    return code;
}

/**
* @functionName  add_state
* @brief         add employee's state
* @author        chenhanlin
* @date          2018-07-30
* @parameter     const QString &username login id
* 				 const int state employee's state
* @return        SUCCESS modify successful
* 				 UPDATE_ERROR modify failed
* 				 EMPTY_QUERY no such employee
* 				 STATE_ERROR no such state
*/
int Employee::add_state(const QString &username, const int state)
{
    CEmployeeDAO dao;
    CEmployee employee;
    int code = dao.query_employee(username, employee);
    if(StatusCode::SQL_EXEC_ERROR == code)
        return StatusCode::UPDATE_ERROR;
    else if(StatusCode::EMPTY_QUERY == code)
        return code;

    if(employee.state() & state){
        return StatusCode::SUCCESS;
    }

    switch (state) {
    case EmployeeState::NEW_EMPLOYEE:
        employee.set_state(employee.state() | EmployeeState::NEW_EMPLOYEE);
        break;
    case EmployeeState::NORMAL:
        employee.set_state(employee.state() | EmployeeState::NORMAL);
        break;
    case EmployeeState::FROZEN:
        employee.set_state(employee.state() | EmployeeState::FROZEN);
        break;
    case EmployeeState::RESIGNATION:
        employee.set_state(employee.state() | EmployeeState::RESIGNATION);
        break;
    default:
        return StatusCode::STATE_ERROR;
    }

    code = dao.save_employee(employee);
    if(StatusCode::SQL_EXEC_ERROR == code)
        return StatusCode::UPDATE_ERROR;
    return code;
}

/**
* @functionName  remove_state
* @brief         remove employee's state
* @author        chenhanlin
* @date          2018-07-30
* @parameter     const QString &username login id
* 				 const int state employee's state
* @return        SUCCESS modify successful
* 				 UPDATE_ERROR modify failed
* 				 EMPTY_QUERY no such employee
* 				 STATE_ERROR no such state
*/
int Employee::remove_state(const QString &username, const int state)
{
    CEmployeeDAO dao;
    CEmployee employee;
    int code = dao.query_employee(username, employee);
    if(StatusCode::SQL_EXEC_ERROR == code)
        return StatusCode::UPDATE_ERROR;
    else if(StatusCode::EMPTY_QUERY == code)
        return code;

    if(!(employee.state() & state)){
        return StatusCode::SUCCESS;
    }

    switch (state) {
    case EmployeeState::NEW_EMPLOYEE:
        employee.set_state(employee.state() ^ EmployeeState::NEW_EMPLOYEE);
        break;
    case EmployeeState::NORMAL:
        employee.set_state(employee.state() ^ EmployeeState::NORMAL);
        break;
    case EmployeeState::FROZEN:
        employee.set_state(employee.state() ^ EmployeeState::FROZEN);
        break;
    case EmployeeState::RESIGNATION:
        employee.set_state(employee.state() ^ EmployeeState::RESIGNATION);
        break;
    default:
        return StatusCode::STATE_ERROR;
    }

    code = dao.save_employee(employee);
    if(StatusCode::SQL_EXEC_ERROR == code)
        return StatusCode::UPDATE_ERROR;
    return code;
}

/**
* @functionName  query_employees
* @brief         the interface of query_employee in dao
* @author        chenhanlin
* @date          2018-08-01
* @parameter     void
* @return        void
*/
int Employee::query_employees(std::vector<CEmployee> &employees, int &total, int page, int page_num, const QString &keyword, int gender, int level, int state)
{
    CEmployeeDAO dao;
    int code = dao.query_employees(employees, total, page, page_num, keyword, gender, level, state);

    if(StatusCode::SQL_EXEC_ERROR == code)
        return StatusCode::QUERY_ERROR;

    return code;
}

/**
* @functionName  log_login
* @brief         record the login employee
* @author        chenhanlin
* @date          2018-08-02
* @parameter     const QString &username login id
* 				 const QString &socket_id connection id
* 				 const QString &level access level
* @return        void
*/
void Employee::log_login(const QString &username, const QString &socket_id, const int level)
{
    user_access.insert(std::map<QString, int>::value_type(username, level));
    socket_id_username.insert(std::map<QString, QString>::value_type(socket_id, username));
    auto iter = login_times.find(username);
    if(login_times.end() != iter){
        login_times.erase(iter);
    }
}

/**
* @functionName  log_login_error_times
* @brief         record the login error times
* @author        chenhanlin
* @date          2018-08-02
* @parameter     const QString &username login id
* @return        void
*/
void Employee::log_login_error_times(const QString &username)
{
    auto iter = login_times.find(username);
    if(login_times.end() != iter){
        iter->second++;
        if(3 == iter->second){
            add_state(username, EmployeeState::FROZEN);
        }
    } else{
        login_times.insert(std::map<QString, int>::value_type(username, 1));
    }

}

/**
* @functionName  logout
* @brief         logout the employee
* @author        chenhanlin
* @date          2018-08-02
* @parameter     const QString &socket_id connection id
* @return        void
*/
void Employee::logout(const QString &socket_id)
{
    auto iter = socket_id_username.find(socket_id);
    if(socket_id_username.end() != iter){
        auto user_access_iter = user_access.find(iter->second);
        user_access.erase(user_access_iter);
        socket_id_username.erase(iter);
    }
}

/**
* @functionName  check_login
* @brief         check whether the employeee is logined or not
* @author        chenhanlin
* @date          2018-08-02
* @parameter     const QString &username login id
* @return        true logined
* 				 false not logined
*/
bool Employee::check_login(const QString &username)
{
    auto iter = user_access.find(username);
    if(user_access.end() == iter){
        return false;
    }else{
        return true;
    }
}

/**
* @functionName  check_access
* @brief         check whether employee has right to do action or not
* @author        chenhanlin
* @date          2018-08-02
* @parameter     const QString &action action name
* 				 const QString &socket_id conenction id
* @return        true can do action
* 				 false cannot do action
*/
bool Employee::check_access(const QString &action, const QString &socket_id)
{
    auto action_iter = action_level.find(action);
    if(action_level.end() == action_iter)
        return false;

    if(0 == action_iter->second) return true;

    auto username_iter = socket_id_username.find(socket_id);
    if(socket_id_username.end() == username_iter)
        return false;

    if(user_access[username_iter->second] >= action_iter->second)
        return true;
    else
        return false;
}

/**
* @functionName  init_access
* @brief         init action and access level table
* @author        chenhanlin
* @date          2018-08-02
* @parameter     void
* @return        void
*/
void Employee::init_access()
{
    QSettings setting(":/app.ini", QSettings::IniFormat);
    setting.beginGroup("actions_level");
    QStringList actions = setting.childKeys();
    for(const auto key : actions){
        action_level.insert(std::map<QString, int>::value_type(key, setting.value(key).toInt()));
    }
    Log::debug("init actions level table");
}
