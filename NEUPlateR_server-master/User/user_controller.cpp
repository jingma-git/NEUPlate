#include "user_controller.h"
#include "userdao.h"
#include "status_code.h"
#include <QDebug>

/**
* @functionName  deduction
* @Description   deduct fee from the user's balance
* @author        chenhanlin
* @date          2018-07-20
* @parameter     const QString &user_id, double fee
* @return        int: QUERY_ERROR : query fail, UPDATE_ERROR: update fail, SUCCESS : excute normal
*/
int User::deduction(const QString &user_id, double fee)
{
    CUserDAO dao;
    CUser user;
    int code = dao.query_user_by_id(user_id, user);
    if(StatusCode::SQL_EXEC_ERROR == code) return StatusCode::QUERY_ERROR;
    user.set_balance(user.balance() + fee);
    code = dao.save_user(user);
    if(StatusCode::SQL_EXEC_ERROR == code) return StatusCode::UPDATE_ERROR;
    return StatusCode::SUCCESS;
}

/**
* @functionName  query_user
* @Description   query user by car's liscense plate
* @author        chenhanlin
* @date          2018-07-20
* @parameter     const QString &liscense
* @return        QString: Empty:query faile, not Empty:query successful
*/
QString User::query_user(const QString &liscense)
{
    CUserDAO dao;
    CUser user;
    int code = dao.query_user_by_license(liscense, user);
    if(StatusCode::SQL_EXEC_ERROR == code || StatusCode::EMPTY_QUERY == code) return QString();
    else return user.user_id();
}

/**
* @functionName  query_balance
* @Description   query user's balance by his user_id
* @author        chenhanlin
* @date          2018-07-20
* @parameter     const QString &user_id
* @return        double:balance
*/
double User::query_balance(const QString &user_id)
{
    CUserDAO dao;
    CUser user;
    int code = dao.query_user_by_id(user_id, user);
    if(StatusCode::SQL_EXEC_ERROR == code || StatusCode::EMPTY_QUERY == code) return 0;
    else return user.balance();
}
