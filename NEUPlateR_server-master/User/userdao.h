#ifndef USERDAO_H
#define USERDAO_H

#include "user.h"
#include <QSqlDatabase>
#include "status_code.h"

class CUserDAO
{
public:
    CUserDAO();
    ~CUserDAO();
    int query_user_by_license(const QString &license, CUser &user);
    int query_user_by_id(const QString &user_id, CUser &user);
    int save_user(CUser &user);
private:
    int insert_user(CUser &user);
    int update_user(const CUser &user);
private:
    QSqlDatabase connection;
};

#endif // USERDAO_H
