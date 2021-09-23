#include "userdao.h"

/**
* @functionName  CUserDAO
* @Description   CUserDAO's constructor, to get database connection
* @author        chenhanlin
* @date          2018-07-23
* @parameter     void
* @return        void
*/
CUserDAO::CUserDAO()
{

}

CUserDAO::~CUserDAO()
{

}

int CUserDAO::query_user_by_license(const QString &license, CUser &user)
{

}

int CUserDAO::query_user_by_id(const QString &user_id, CUser &user)
{

}

/**
* @functionName  save_user
* @Description   save user's information into database
* @author        chenhanlin
* @date          2018-07-20
* @parameter     CUser &user
* @return        SUCCESS:save successful, SQL_EXEC_ERROR: save failed
*/
int CUserDAO::save_user(CUser &user)
{
    if(user.is_new()){
        return this->insert_user(user);
    }else{
        return this->update_user(user);
    }
}

int CUserDAO::insert_user(CUser &user)
{

}

int CUserDAO::update_user(const CUser &user)
{

}
