#ifndef QUESTION_CONTROLLER_H
#define QUESTION_CONTROLLER_H

#include <QString>
#include <map>

namespace Question {
int set_security_question(const QString &username, const std::map<QString, QString> &questions);
int get_security_questions(std::map<QString, QString> &questions);
int get_employee_question(const QString &username, std::map<QString, QString> &questions);
int check_employee_answer(const QString &username, const QString &q_id, const QString &answer);
}

#endif // QUESTION_CONTROLLER_H
