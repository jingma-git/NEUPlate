#ifndef QUESTION_DAO_H
#define QUESTION_DAO_H

#include <QSqlDatabase>
#include <vector>
#include <map>
#include "RedisClient/hiredispp.h"

class CQuestionDAO
{
public:
    CQuestionDAO();
    CQuestionDAO(const QSqlDatabase &conn);
    ~CQuestionDAO();
    int query_employee_questions(const QString &username, std::map<QString, QString> &questions);
    int query_questions(std::map<QString, QString> &questions);
    int query_question_answer(const QString &username, const QString &q_id, QString &answer);
    int save_employee_question(const QString &username, const std::map<QString, QString> questions);
    int delete_employee_question(const QString &username);
private:
    QSqlDatabase m_db_conn;
    hiredispp::Redis m_redis_conn;
    bool m_my_release;
};

#endif // QUESTION_DAO_H
