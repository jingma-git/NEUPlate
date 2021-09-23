#ifndef USER_H
#define USER_H

#include <QString>


class CUser
{
public:
    CUser();
    CUser(const QString &u_id, const QString &username, const QString &passwd, const QString &name, const double balance);
    QString user_id() const;
    void set_user_id(const QString &user_id);

    QString username() const;
    void set_username(const QString &username);

    QString passwd() const;
    void set_passwd(const QString &passwd);

    QString name() const;
    void set_name(const QString &name);

    double balance() const;
    void set_balance(double balance);

    bool is_new() const;

private:
    QString m_user_id;
    QString m_username;
    QString m_passwd;
    QString m_name;
    double m_balance;
    bool m_is_new;
};

#endif // USER_H
