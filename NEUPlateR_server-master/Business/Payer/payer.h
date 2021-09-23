#ifndef PAYER_H
#define PAYER_H

#include "QString"
#include <QJsonObject>

class CPayer
{
public:
    CPayer();
    CPayer(const QJsonObject &payer_json);
    CPayer(const QString &payer_id, const QString &user_name,
           const QString &name, const QString &id_card, const QString &tel,
           const QString &password, const QString &photo, const double &balance,
           const QString &birthday, const int gender);


    QString name() const;
    void setName(const QString &name);

    QString tel() const;
    void setTel(const QString &tel);

    QString payer_id() const;
    void setPayer_id(const QString &payer_id);

    QString password() const;
    void setPassword(const QString &password);

    QString username() const;
    void setUsername(const QString &username);

    QString id_card() const;
    void setId_card(const QString &id_card);

    double balance() const;
    void setBalance(double balance);

    QString photo() const;
    void setPhoto(const QString &photo);

    QString birthday() const;
    void setBirthday(const QString &birthday);

    int gender() const;
    void setGender(int gender);

    QJsonObject toJSON() const;




private:

    QString m_payer_id;
    QString m_username;
    QString m_name;
    QString m_id_card;
    QString m_tel;
    QString m_password;
    QString m_photo;
    double m_balance;
    QString m_birthday;
    int m_gender;

};


#endif // PAYER_H
