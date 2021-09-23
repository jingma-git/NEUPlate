#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include "user.h"

namespace User {
//int query_balance(const QString &user_id, double &balance);
int deduction(const QString &user_id, double fee);
QString query_user(const QString &liscense);
double query_balance(const QString &user_id);
}


#endif // USER_CONTROLLER_H
