#ifndef EMPLOYEE_CONTROLLER_H
#define EMPLOYEE_CONTROLLER_H

#include <QDate>
#include <vector>
#include <map>
#include <set>
#include "employee.h"

namespace Employee {
int add_employee(const QString &name, const QString &passwd, const int gender, const QString &id_card,
                 const QString &photo, const int access_level, const QString &work_place="", const QDate &birthday=QDate::fromString("1970-01-01", "yyyy-MM-dd"),
                 const QString &birth_place="", const bool married=false, const QString &nation="", const int edu=-1,
                 const QString &telephone="", const QString &email="", const QString &political_state="");
int delete_employees(const std::vector<QString> &usernames);
int query_employee(const QString &username, CEmployee &employee);
int modify_employee(const CEmployee &employee);
int add_state(const QString &username, const int state);
int remove_state(const QString &username, const int state);
int query_employees(std::vector<CEmployee> &employees, int &total, int page, int page_num, const QString &keyword, int gender, int level, int state);
void log_login(const QString &username, const QString &socket_id, const int level);
void log_login_error_times(const QString &username);
void logout(const QString &socket_id);
bool check_login(const QString& username);
bool check_access(const QString &action, const QString &socket_id);
void init_access();
namespace {
std::map<QString, int> login_times;
std::map<QString, int> user_access;
std::map<QString, QString> socket_id_username;
std::map<QString, int> action_level;
}
}

#endif // EMPLOYEE_CONTROLLER_H
