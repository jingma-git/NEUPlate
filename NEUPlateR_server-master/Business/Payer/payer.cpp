#include "payer.h"

CPayer::CPayer()
{


}

CPayer::CPayer(const QJsonObject &payer_json)
{

    //The returned QJsonValue is QJsonValue::Undefined if the key does not exist.
    m_payer_id=payer_json.value("payer_id").toString();
    m_username=payer_json.value("username").toString();
    m_name=payer_json.value("name").toString();
    m_id_card=payer_json.value("id_card").toString();
    m_tel=payer_json.value("tel").toString();
    m_password=payer_json.value("password").toString();
    m_photo=payer_json.value("photo").toString();
    m_balance=payer_json.value("balance").toDouble();
    m_birthday=payer_json.value("birthday").toString();
    m_gender=payer_json.value("gender").toInt();
}

CPayer::CPayer(const QString &payer_id, const QString &user_name,
               const QString &name, const QString &id_card, const QString &tel,
               const QString &password, const QString &photo, const double &balance, const QString &birthday, const int gender):
                m_payer_id(payer_id),m_username(user_name),m_name(name),m_id_card(id_card),
                m_tel(tel),m_password(password),m_photo(photo),m_balance(balance),m_birthday(birthday),m_gender(gender)
{

}


QString CPayer::name() const
{
    return m_name;
}

void CPayer::setName(const QString &name)
{
    m_name = name;
}

QString CPayer::tel() const
{
    return m_tel;
}

void CPayer::setTel(const QString &tel)
{
    m_tel = tel;
}

QString CPayer::payer_id() const
{
    return m_payer_id;
}

void CPayer::setPayer_id(const QString &payer_id)
{
    m_payer_id = payer_id;
}

QString CPayer::password() const
{
    return m_password;
}

void CPayer::setPassword(const QString &password)
{
    m_password = password;
}

QString CPayer::username() const
{
    return m_username;
}

void CPayer::setUsername(const QString &username)
{
    m_username = username;
}

QString CPayer::id_card() const
{
    return m_id_card;
}

void CPayer::setId_card(const QString &id_card)
{
    m_id_card = id_card;
}

double CPayer::balance() const
{
    return m_balance;
}

void CPayer::setBalance(double balance)
{
    m_balance = balance;
}

QString CPayer::photo() const
{
    return m_photo;
}

void CPayer::setPhoto(const QString &photo)
{
    m_photo = photo;
}

QString CPayer::birthday() const
{
    return m_birthday;
}

void CPayer::setBirthday(const QString &birthday)
{
    m_birthday = birthday;
}

int CPayer::gender() const
{
    return m_gender;
}

void CPayer::setGender(int gender)
{
    m_gender = gender;
}

QJsonObject CPayer::toJSON() const
{
    QJsonObject payer_json;
//    payer_json.insert("payer_id",m_payer_id);
//    payer_json.insert("username",m_username);
//    payer_json.insert("name",m_name);
//    payer_json.insert("id_card",m_id_card);
//    payer_json.insert("tel",m_tel);
//    payer_json.insert("password",m_password);
//    payer_json.insert("photo",m_photo);
//    payer_json.insert("balance",m_balance);
//    payer_json.insert("birthday",m_birthday);
//    payer_json.insert("gender",m_gender);
    payer_json.insert("payer_id",payer_id());
    payer_json.insert("username",username());
    payer_json.insert("name",name());
    payer_json.insert("id_card",id_card());
    payer_json.insert("tel",tel());
    payer_json.insert("password",password());
    payer_json.insert("photo",photo());
    payer_json.insert("balance",balance());
    payer_json.insert("birthday",birthday());
    payer_json.insert("gender",gender());
    return payer_json;
}



