#ifndef EMPLOYEE_DAP_H
#define EMPLOYEE_DAP_H

#include <QSqlDatabase>
#include <vector>
#include "RedisClient/hiredispp.h"
#include "employee.h"

class CEmployeeDAO
{
public:
    CEmployeeDAO();
    CEmployeeDAO(const QSqlDatabase &db);
    ~CEmployeeDAO();

    int save_employee(const CEmployee &employee);
    int delete_employee(const QString &username);
    int query_employee(const QString &username, CEmployee &employee);
    int query_employees(std::vector<CEmployee> &employees, int &total, int page, int page_num, const QString &keyword, int gender, int level, int state);
private:
    int insert_employee(const CEmployee &employee);
    int update_employee(const CEmployee &employee);
private:
    QSqlDatabase m_db_conn;
    hiredispp::Redis m_redis_conn;
    bool m_my_release;
};

#endif // EMPLOYEE_DAP_H
