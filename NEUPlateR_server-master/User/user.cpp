#include "user.h"

CUser::CUser()
{

}

CUser::CUser(const QString &u_id, const QString &username, const QString &passwd, const QString &name, const double balance)
    :m_user_id(u_id), m_username(username), m_passwd(passwd),
      m_name(name), m_balance(balance), m_is_new(false)
{

}

QString CUser::user_id() const
{
    return m_user_id;
}

void CUser::set_user_id(const QString &user_id)
{
    m_user_id = user_id;
}

QString CUser::username() const
{
    return m_username;
}

void CUser::set_username(const QString &username)
{
    m_username = username;
}

QString CUser::passwd() const
{
    return m_passwd;
}

void CUser::set_passwd(const QString &passwd)
{
    m_passwd = passwd;
}

QString CUser::name() const
{
    return m_name;
}

void CUser::set_name(const QString &name)
{
    m_name = name;
}

double CUser::balance() const
{
    return m_balance;
}

void CUser::set_balance(double balance)
{
    m_balance = balance;
}

bool CUser::is_new() const
{
    return m_is_new;
}
