#include "question_dao.h"
#include "pool/connection_pool.h"
#include "neu_head.h"
#include <QSqlQuery>
#include <iostream>

CQuestionDAO::CQuestionDAO()
      :m_redis_conn("localhost"),
      m_my_release(true)
{
    
}

CQuestionDAO::CQuestionDAO(const QSqlDatabase &conn)
    :m_db_conn(conn),
      m_redis_conn("localhost"),
      m_my_release(false)
{
    
}

CQuestionDAO::~CQuestionDAO()
{
    if(m_my_release)
        ConnectionPool::close_connection(m_db_conn);
}

/** 
* @functionName  query_employee_question
* @brief         query a employee's security question by username
* @author        chenhanlin
* @date          2018-07-29
* @parameter     const QString &username user login id
* 				 std::map<QString, QString> &question output save question and its id
* @return        SUCCESS query successful
* 				 SQL_EXEC_ERROR query failed
* 				 EMPTY_QUERY user's question is empty
*/
int CQuestionDAO::query_employee_questions(const QString &username, std::map<QString, QString> &questions)
{
    // query redis first
    std::string key("questions_"+username.toStdString());
    if(m_redis_conn.exists("questions") && m_redis_conn.exists(key)){
        hiredispp::Redis::Reply reply = m_redis_conn.smembers(key);
        std::vector<std::string> keys;
        reply.toVector(keys);
        for(const auto &k : keys){
            questions.insert(std::map<QString, QString>::value_type(k.c_str(), m_redis_conn.hget("questions", k).c_str()));
        }
        return StatusCode::SUCCESS;
    }
    
    // query db
    if(!m_db_conn.isOpen()){
        m_db_conn = ConnectionPool::open_connection();
    }
    QSqlQuery query(m_db_conn);
    query.prepare("SELECT q_id, question FROM employee_question_answer WHERE username=:username");
    query.bindValue(":username", username);
    
    bool success = query.exec();
    
    if(!success) return StatusCode::SQL_EXEC_ERROR;
    
    while (query.next()) {
        questions.insert(std::map<QString, QString>::value_type(query.value("q_id").toString(), query.value("question").toString()));
    }
    
    if(questions.empty()) return StatusCode::EMPTY_QUERY;

    for(const auto &q : questions){
        m_redis_conn.sadd(key, q.first.toStdString());
    }
    
    return StatusCode::SUCCESS;
}

/** 
* @functionName  query_questions
* @brief         query all security question in system
* @author        chenhanlin
* @date          2018-07-29
* @parameter     std::map<QString, QString> &questions save question id and desc
* @return        SUCCESS query success
* 				 SQL_EXEC_ERROR query failed
* 				 EMPTY_QUERY no question in system now
*/
int CQuestionDAO::query_questions(std::map<QString, QString> &questions)
{
    std::string key("questions");
    // query redis first
    if(m_redis_conn.exists(key)){
        hiredispp::Redis::Command get_keys("HKEYS");
        get_keys << "questions";
        hiredispp::Redis::Reply reply = m_redis_conn.doCommand(get_keys);
        std::vector<std::string> keys;
        reply.toVector(keys);
        for(const auto &k : keys){
            questions.insert(std::map<QString, QString>::value_type(k.c_str(), m_redis_conn.hget(key, k).c_str()));
        }
        return StatusCode::SUCCESS;
    }
    
    // query db
    if(!m_db_conn.isOpen()){
        m_db_conn = ConnectionPool::open_connection();
    }
    QSqlQuery query(m_db_conn);
    query.prepare("SELECT * FROM security_question");
    bool success = query.exec();
    
    if(!success) return StatusCode::SQL_EXEC_ERROR;
    
    while(query.next()){
        questions.insert(std::map<QString, QString>::value_type(query.value("q_id").toString(), query.value("question").toString()));
    }
    
    if(questions.empty()) return StatusCode::EMPTY_QUERY;

    for(const auto &q : questions){
        m_redis_conn.hset(key, q.first.toStdString(), q.second.toStdString());
    }
    
    return StatusCode::SUCCESS;
}

/** 
* @functionName  query_question_answer
* @brief         query user's security question answer
* @author        chenhanlin
* @date          2018-07-29
* @parameter     const QString &username user's login id
* 				 const QString &q_id question id
* 				 QString &answer question's answer
* @return        SUCCESS query successful
* 			 	 SQL_EXEC_ERROR query failed
* 				 EMPTY_QUERY no answer
*/
int CQuestionDAO::query_question_answer(const QString &username, const QString &q_id, QString &answer)
{
    // query redis first
    std::string key("answer_"+username.toStdString()+"_"+q_id.toStdString());
    if(m_redis_conn.exists(key)){
        answer = m_redis_conn.get(key).c_str();
        return StatusCode::SUCCESS;
    }

    // query db
    if(!m_db_conn.isOpen()){
        m_db_conn = ConnectionPool::open_connection();
    }
    QSqlQuery query(m_db_conn);
    query.prepare("SELECT answer FROM employee_question_answer WHERE username=:username AND q_id=:q_id");
    
    query.bindValue(":username", username);
    query.bindValue(":q_id", q_id);
    
    bool success = query.exec();
    if(!success) return StatusCode::SQL_EXEC_ERROR;
    
    if(query.next()){
        answer = query.value("answer").toString();
    }else{
        return StatusCode::EMPTY_QUERY;
    }

    m_redis_conn.set(key, answer.toStdString());
    
    return StatusCode::SUCCESS;
}

/** 
* @functionName  save_employee_question
* @brief         save user's security question
* @author        chenhanlin
* @date          2018-07-29
* @parameter     const QString &username user's login id
* 				 const std::vector<QString, QString> questions user's security question
* @return        SUCCESS save successful
* 				 SQL_EXEC_ERROR save failed
*/
int CQuestionDAO::save_employee_question(const QString &username, const std::map<QString, QString> questions)
{
    if(!m_db_conn.isOpen()){
        m_db_conn = ConnectionPool::open_connection();
    }
    QSqlQuery save(m_db_conn);
    
    // delete first
    this->delete_employee_question(username);
    
    save.prepare("INSERT INTO employee_question VALUES(:q_id, :e_id, :answer)");
    bool success = false;
    
    for(const auto &q : questions){
        save.bindValue(":q_id", q.first);
        save.bindValue(":e_id", username);
        save.bindValue(":answer", q.second);
        success = save.exec();
        if(!success)
            return StatusCode::SQL_EXEC_ERROR;
    }
    
    // write to redis
    std::string key("questions_"+username.toStdString());
    for(const auto &q : questions){
        m_redis_conn.sadd(key, q.first.toStdString());
        m_redis_conn.set("answer_"+username.toStdString()+"_"+q.first.toStdString(), q.second.toStdString());
    }
    
    // return code
    return StatusCode::SUCCESS;
}

/** 
* @functionName  delete_employee_question
* @brief         delete user's security question
* @author        chenhanlin
* @date          2018-07-29
* @parameter     const QString &username user's login id
* @return        SUCCESS delete successful
* 				 SQL_EXEC_ERROR delete failed
*/
int CQuestionDAO::delete_employee_question(const QString &username)
{
    if(!m_db_conn.isOpen()){
        m_db_conn = ConnectionPool::open_connection();
    }
    QSqlQuery remove(m_db_conn);
    remove.prepare("DELETE FROM employee_question WHERE e_id=:e_id");
    remove.bindValue(":e_id", username);
    bool success = remove.exec();
    
    if(!success)
        return StatusCode::SQL_EXEC_ERROR;
    
    // delete redis
    m_redis_conn.del("questions_"+username.toStdString());
    auto reply = m_redis_conn.keys("answer_"+username.toStdString()+"_*");
    std::vector<std::string> keys;
    reply.toVector(keys);
    for(const auto k : keys)
        m_redis_conn.del(k);

    // return code
    return StatusCode::SUCCESS;
}
