#include "question_controller.h"
#include "status_code.h"
#include "question_dao.h"
#include "neu_head.h"

/**
* @functionName  set_security_question
* @brief         set employee's security question
* @author        chenhanlin
* @date          2018-07-29
* @parameter     const QString &username user's login id
* 				 const std::vector<QString, QString> questions user's security question
* @return        SUCCESS save successful
* 				 INSERT_ERROR save failed
*/
int Question::set_security_question(const QString &username, const std::map<QString, QString> &questions)
{
    CQuestionDAO dao;
    int code = dao.save_employee_question(username, questions);
    if(StatusCode::SUCCESS != code)
        return StatusCode::INSERT_ERROR;
    return StatusCode::SUCCESS;
}

/**
* @functionName  get_security_questions
* @brief         query all security question in system
* @author        chenhanlin
* @date          2018-07-29
* @parameter     std::map<QString, QString> &questions save question id and desc
* @return        SUCCESS query success
* 				 QUERY_ERROR query failed
* 				 EMPTY_QUERY no question in system now
*/
int Question::get_security_questions(std::map<QString, QString> &questions)
{
    CQuestionDAO dao;
    int code = dao.query_questions(questions);
    if(StatusCode::SQL_EXEC_ERROR == code)
        return StatusCode::QUERY_ERROR;
    return code;
}

/**
* @functionName  get_employee_question
* @brief         query a employee's security question by username
* @author        chenhanlin
* @date          2018-07-29
* @parameter     const QString &username user login id
* 				 std::map<QString, QString> &question output save question and its id
* @return        SUCCESS query successful
* 				 QUERY_ERROR query failed
* 				 EMPTY_QUERY user's question is empty
*/
int Question::get_employee_question(const QString &username, std::map<QString, QString> &questions)
{
    CQuestionDAO dao;
    int code = dao.query_employee_questions(username, questions);
    if(StatusCode::SQL_EXEC_ERROR == code)
        return StatusCode::QUERY_ERROR;
    return code;
}

/**
* @functionName  check_employee_answer
* @brief         check whether employee's ansewer is right or not
* @author        chenhanlin
* @date          2018-07-29
* @parameter     const QString &username employee's login id
* 				 const QString &q_id question id
* 				 const QString &answer question answer
* @return        RIGHT_ANSWER answer is right
* 				 WRONG_ANSWER answer is wrong
* 				 QUERY_ERROR query failed
* 				 EMPTY_QUERY query nothing
*/
int Question::check_employee_answer(const QString &username, const QString &q_id, const QString &answer)
{
    CQuestionDAO dao;
    QString correct;
    int code = dao.query_question_answer(username, q_id, correct);
    if(StatusCode::SQL_EXEC_ERROR == code)
        return StatusCode::QUERY_ERROR;
    else if(StatusCode::EMPTY_QUERY == code)
        return code;

    if(answer == correct){
        return StatusCode::RIGHT_ANSWER;
    } else{
        return StatusCode::WRONG_ANSWER;
    }
}
