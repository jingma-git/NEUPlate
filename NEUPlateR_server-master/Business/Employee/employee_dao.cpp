#include "employee_dao.h"
#include "pool/connection_pool.h"
#include "neu_head.h"
#include <QSqlQuery>
#include <QDate>

CEmployeeDAO::CEmployeeDAO()
      :m_redis_conn("localhost"),
      m_my_release(true)
{

}

CEmployeeDAO::CEmployeeDAO(const QSqlDatabase &db)
    :m_db_conn(db),
      m_redis_conn("localhost"),
      m_my_release(false)
{
}

CEmployeeDAO::~CEmployeeDAO()
{
    if(m_my_release){
        ConnectionPool::close_connection(m_db_conn);
    }
}

/**
* @functionName  save_employee
* @brief         save employee information into db
* @author        chenhanlin
* @date          2018-07-27
* @parameter     const CEmployee &employee Employee Entitiy
* @return        SUCCESS save successful
* 				 SQL_EXEC_ERROR save failed
*/
int CEmployeeDAO::save_employee(const CEmployee &employee)
{
    int code = 0;
    if(employee.is_new()){
        code = this->insert_employee(employee);
    }else{
        code = this->update_employee(employee);
    }
    if(StatusCode::SUCCESS == code){
        // write redis
        std::string key(QString("employee_%1").arg(employee.username()).toStdString());
        m_redis_conn.hset(key, "name", employee.name().toStdString());
        m_redis_conn.hset(key, "password", employee.password().toStdString());
        m_redis_conn.hset(key, "username", employee.username().toStdString());
        m_redis_conn.hset(key, "gender", std::to_string(employee.gender()));
        m_redis_conn.hset(key, "id", employee.id().toStdString());
        m_redis_conn.hset(key, "state", std::to_string(employee.state()));
        m_redis_conn.hset(key, "photo_path", employee.photo_path().toStdString());
        m_redis_conn.hset(key, "work_place", employee.work_place().toStdString());
        m_redis_conn.hset(key, "birthday", employee.birthday().toStdString());
        m_redis_conn.hset(key, "birth_place", employee.birth_place().toStdString());
        m_redis_conn.hset(key, "married", std::to_string(int(employee.married())));
        m_redis_conn.hset(key, "nation", employee.nation().toStdString());
        m_redis_conn.hset(key, "education", std::to_string(employee.education()));
        m_redis_conn.hset(key, "telephone", employee.telephone().toStdString());
        m_redis_conn.hset(key, "political_state", employee.political_state().toStdString());
        m_redis_conn.hset(key, "access_level", std::to_string(employee.access_level()));
        m_redis_conn.hset(key, "email", employee.email().toStdString());
    }
    return code;
}

/**
* @functionName  delete_employee
* @brief         delete employee from database
* @author        chenhanlin
* @date          2018-07-27
* @parameter     const QString &username employee's work id
* @return        SUCCESS delete success
* 				 DELETE_ERROR delete failed
*/
int CEmployeeDAO::delete_employee(const QString &username)
{
    if(!m_db_conn.isOpen()){
        m_db_conn = ConnectionPool::open_connection();
    }
    QSqlQuery remove(m_db_conn);
    remove.prepare("CALL delete_employee(:username, @error)");
    remove.bindValue(":username", username);
    remove.exec();
    remove.exec("SELECT @error");
    remove.next();
    int code = remove.value(0).toInt();
    if(1 == code){
        return StatusCode::DELETE_ERROR;
    }
    // delete from redis
    m_redis_conn.del(QString("employee_%1").arg(username).toStdString());
    m_redis_conn.del("questions_"+username.toStdString());
    auto reply = m_redis_conn.keys("answer_"+username.toStdString()+"_*");
    std::vector<std::string> keys;
    reply.toVector(keys);
    for(const auto k : keys)
        m_redis_conn.del(k);
    // return code
    return StatusCode::SUCCESS;
}

/**
* @functionName  query_employee
* @brief         query a employee's information by his username
* @author        chenhanlin
* @date          2018-07-29
* @parameter     const QString &username employee's login id
* 				 CEmployee &employee employee's entity
* @return        SUCCESS query success
* 				 SQL_EXEC_ERROR execute sql error
* 				 EMPTY_QUERY no such employee
*/
int CEmployeeDAO::query_employee(const QString &username, CEmployee &employee)
{
    // query redis first
    std::string key("employee_"+username.toStdString());
    if(m_redis_conn.exists(key)){
        employee.set_name(m_redis_conn.hget(key, "name").c_str());
        employee.set_password(m_redis_conn.hget(key, "password").c_str());
        employee.set_username(m_redis_conn.hget(key, "username").c_str());
        employee.set_gender(std::stoi(m_redis_conn.hget(key, "gender")));
        employee.set_id(m_redis_conn.hget(key, "id").c_str());
        employee.set_state(std::stoi(m_redis_conn.hget(key, "state")));
        employee.set_photo_path(m_redis_conn.hget(key, "photo_path").c_str());
        employee.set_work_place(m_redis_conn.hget(key, "work_place").c_str());
        employee.set_birthday(QDate::fromString(m_redis_conn.hget(key, "birthday").c_str()));
        employee.set_birth_place(m_redis_conn.hget(key, "birth_place").c_str());
        employee.set_married(bool(std::stoi(m_redis_conn.hget(key, "married"))));
        employee.set_nation(m_redis_conn.hget(key, "nation").c_str());
        employee.set_education(std::stoi(m_redis_conn.hget(key, "education")));
        employee.set_telephone(m_redis_conn.hget(key, "telephone").c_str());
        employee.set_political_state(m_redis_conn.hget(key, "political_state").c_str());
        employee.set_access_level(std::stoi(m_redis_conn.hget(key, "access_level")));
        employee.set_email(m_redis_conn.hget(key, "email").c_str());
        employee.set_is_new(false);
        return StatusCode::SUCCESS;
    }

    if(!m_db_conn.isOpen()){
        m_db_conn = ConnectionPool::open_connection();
    }
    // query database
    QSqlQuery query(m_db_conn);
    query.prepare("SELECT * FROM employee_info WHERE username=:username");
    query.bindValue(":username", username);
    bool success = query.exec();
    if(!success) return StatusCode::SQL_EXEC_ERROR;

    if(query.next()){
        // get data and put into entity
        employee.set_name(query.value("name").toString());
        m_redis_conn.hset(key, "name", employee.name().toStdString());

        employee.set_password(query.value("password").toString());
        m_redis_conn.hset(key, "password", employee.password().toStdString());

        employee.set_username(query.value("username").toString());
        m_redis_conn.hset(key, "username", employee.username().toStdString());

        employee.set_gender(query.value("gender").toInt());
        m_redis_conn.hset(key, "gender", std::to_string(employee.gender()));

        employee.set_id(query.value("id_card").toString());
        m_redis_conn.hset(key, "id", employee.id().toStdString());

        employee.set_state(query.value("state").toInt());
        m_redis_conn.hset(key, "state", std::to_string(employee.state()));

        employee.set_photo_path(query.value("photo").toString());
        m_redis_conn.hset(key, "photo_path", employee.photo_path().toStdString());

        employee.set_work_place(query.value("work_place").toString());
        m_redis_conn.hset(key, "work_place", employee.work_place().toStdString());

        employee.set_birthday(QDate::fromString(query.value("birthday").toString()));
        m_redis_conn.hset(key, "birthday", employee.birthday().toStdString());

        employee.set_birth_place(query.value("birth_place").toString());
        m_redis_conn.hset(key, "birth_place", employee.birth_place().toStdString());

        employee.set_married(query.value("married").toBool());
        m_redis_conn.hset(key, "married", std::to_string(int(employee.married())));

        employee.set_nation(query.value("nation").toString());
        m_redis_conn.hset(key, "nation", employee.nation().toStdString());

        employee.set_education(query.value("education").toInt());
        m_redis_conn.hset(key, "education", std::to_string(employee.education()));

        employee.set_telephone(query.value("telephone").toString());
        m_redis_conn.hset(key, "telephone", employee.telephone().toStdString());

        employee.set_political_state(query.value("political_state").toString());
        m_redis_conn.hset(key, "political_state", employee.political_state().toStdString());

        employee.set_access_level(query.value("access_level").toInt());
        m_redis_conn.hset(key, "access_level", std::to_string(employee.access_level()));

        employee.set_email(query.value("email").toString());
        m_redis_conn.hset(key, "email", employee.email().toStdString());

        employee.set_is_new(false);
        return StatusCode::SUCCESS;
    }else{
        return StatusCode::EMPTY_QUERY;
    }

}

/**
* @functionName  query_employees
* @brief         query employees by some condition
* @author        chenhanlin
* @date          2018-08-01
* @parameter     std::vector<CEmployee> &employees contain the employee entities and output
* 				 int page the number of page you want query
* 				 int page_num the number of items in a page
* 				 const QString &keyword query keyword
* 				 int gender query by gender
* 				 int level query by access level
* @return        SUCEESS query successful
* 				 SQL_EXEC_ERROR query failed
* 				 EMPTY_QUERY query empty
*/
int CEmployeeDAO::query_employees(std::vector<CEmployee> &employees, int &total, int page, int page_num, const QString &keyword, int gender, int level, int state)
{
    if(!m_db_conn.isOpen()){
        m_db_conn = ConnectionPool::open_connection();
    }
    QSqlQuery query(m_db_conn);
    QStringList condition;
    QString sql = "SELECT SQL_CALC_FOUND_ROWS * FROM employee_info";
    if(!keyword.isEmpty() || 1 == gender || 2 == gender || level > 0 || state > 0){
        sql += " WHERE ";
    }

    if(!keyword.isEmpty()){
        condition << QString("CONCAT(IFNULL(name, ''), IFNULL(username, ''), IFNULL(id_card, ''), IFNULL(work_place, '')) LIKE '%1'").arg("%"+keyword+"%");
    }

    if(1 == gender || 2 == gender){
        condition << QString("gender = %1").arg(gender);
    }

    if(level > 0){
        condition << QString("access_level = %1").arg(level);
    }

    if(state > 0){
        condition << QString("state & %1").arg(state);
    }

    if(!condition.isEmpty())
        sql += condition.join(" AND ");

    sql += QString(" LIMIT %1, %2").arg(page < 0 ? 0 : page*page_num).arg(page_num);

    bool success = query.exec(sql);
    if(!success){
        Log::debug("query employees error");
        return StatusCode::SQL_EXEC_ERROR;
    }

    while(query.next()){
        CEmployee employee;
        employee.set_name(query.value("name").toString());
        employee.set_password(query.value("password").toString());
        employee.set_username(query.value("username").toString());
        employee.set_gender(query.value("gender").toInt());
        employee.set_id(query.value("id_card").toString());
        employee.set_state(query.value("state").toInt());
        employee.set_photo_path(query.value("photo").toString());
        employee.set_work_place(query.value("work_place").toString());
        employee.set_birthday(QDate::fromString(query.value("birthday").toString()));
        employee.set_birth_place(query.value("birth_place").toString());
        employee.set_married(query.value("married").toBool());
        employee.set_nation(query.value("nation").toString());
        employee.set_education(query.value("education").toInt());
        employee.set_telephone(query.value("telephone").toString());
        employee.set_political_state(query.value("political_state").toString());
        employee.set_access_level(query.value("access_level").toInt());
        employee.set_email(query.value("email").toString());
        employee.set_is_new(false);
        employees.push_back(employee);
    }

    success = query.exec("SELECT FOUND_ROWS()");
    if(!success)
        return StatusCode::SQL_EXEC_ERROR;

    if(query.next()){
        total = query.value(0).toInt();
    }else{
        total = 0;
    }

    if(employees.empty())
        return StatusCode::EMPTY_QUERY;

    return StatusCode::SUCCESS;

}

/**
* @functionName  insert_employee
* @brief         insert a new record into database
* @author        chenhanlin
* @date          2018-07-29
* @parameter     const CEmployee &employee Employee's enetity
* @return        SUCCESS insert successful
* 				 SQL_EXEC_ERROR insert failed
*/
int CEmployeeDAO::insert_employee(const CEmployee &employee)
{
    if(!m_db_conn.isOpen()){
        m_db_conn = ConnectionPool::open_connection();
    }
    QSqlQuery insert(m_db_conn);
    insert.prepare("CALL add_employee(:name, :passwd, :gender, :id_card, :state, :photo, :work_place, :birthday, :birth_place, "
                   ":married, :nation, :education, :telephone, :email, :political_state, :access, @error_code)");
    insert.bindValue(":name", employee.name());
    insert.bindValue(":passwd", employee.password());
    insert.bindValue(":gender", employee.gender());
    insert.bindValue(":id_card", employee.id());
    insert.bindValue(":state", employee.state());
    insert.bindValue(":photo", employee.photo_path());
    insert.bindValue(":work_place", employee.work_place());
    insert.bindValue(":birthday", employee.birthday());
    insert.bindValue(":birth_place", employee.birth_place());
    insert.bindValue(":married", int(employee.married()));
    insert.bindValue(":nation", employee.nation());
    insert.bindValue(":education", employee.education());
    insert.bindValue(":telephone", employee.telephone());
    insert.bindValue(":email", employee.email());
    insert.bindValue(":political_state", employee.political_state());
    insert.bindValue(":access", employee.access_level());


    insert.exec();
    insert.exec("SELECT @error_code;");
    insert.next();
    int success = insert.value(0).toInt();
    if(1 == success){
        Log::debug(insert.lastError().text());
        return StatusCode::SQL_EXEC_ERROR;
    }else{
        return StatusCode::SUCCESS;
    }
}

int CEmployeeDAO::update_employee(const CEmployee &employee)
{
    if(!m_db_conn.isOpen()){
        m_db_conn = ConnectionPool::open_connection();
    }
    QSqlQuery update(m_db_conn);
    update.prepare("UPDATE employee_info SET name=:name, password=:passwd, gender=:gender, "
                   "id_card=:id_card, state=:state, photo=:photo, work_place=:work_place, "
                   "birthday=:birthday, birth_place=:birth_place, married=:married, nation=:nation, "
                   "education=:edu, telephone=:telephone, email=:email, political_state=:political_state, access_level=:access"
                   " WHERE username=:username");

    update.bindValue(":name", employee.name());
    update.bindValue(":passwd", employee.password());
    update.bindValue(":gender", employee.gender());
    update.bindValue(":id_card", employee.id());
    update.bindValue(":state", employee.state());
    update.bindValue(":photo", employee.photo_path());
    update.bindValue(":work_place", employee.work_place());
    update.bindValue(":birthday", employee.birthday());
    update.bindValue(":birth_place", employee.birth_place());
    update.bindValue(":married", int(employee.married()));
    update.bindValue(":nation", employee.nation());
    update.bindValue(":edu", employee.education());
    update.bindValue(":telephone", employee.telephone());
    update.bindValue(":email", employee.email());
    update.bindValue(":political_state", employee.political_state());
    update.bindValue(":access", employee.access_level());
    update.bindValue(":username", employee.username());

    bool success = update.exec();
    if(!success){
        Log::debug(update.lastError().text());
        return StatusCode::SQL_EXEC_ERROR;
    }else{
        return StatusCode::SUCCESS;
    }
}
