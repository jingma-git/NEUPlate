#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QDate>
#include <QJsonObject>

namespace Gender {
static const int MALE = 1;
static const int FEMALE = 2;
}

namespace Education {
static const int SECRET = 0;
static const int PRIMARY_SCHOOL = 1;
static const int SECONDARY_SCHOOL = 2;
static const int HIGH_SCHOOL = 3;
static const int SPECIALIST_QUALIFICATIONS = 4;
static const int BACHELOR = 5;
static const int POSTGRADUATE = 6;
static const int DOCTOR = 7;
}

namespace EmployeeState {
static const int NEW_EMPLOYEE = 1;
static const int NORMAL = 2;
static const int FROZEN = 4;
static const int RESIGNATION = 8;
}

class CEmployee
{
public:
    CEmployee();
    QString name() const;
    void set_name(const QString &name);

    QString username() const;
    void set_username(const QString &username);

    QString password() const;
    void set_password(const QString &password);

    QString id() const;
    void set_id(const QString &id);

    int gender() const;
    void set_gender(int gender);

    QString photo_path() const;
    void set_photo_path(const QString &photo_path);

    QString work_place() const;
    void set_work_place(const QString &work_place);

    QString birthday() const;
    void set_birthday(const QDate &birthday);

    QString birth_place() const;
    void set_birth_place(const QString &birth_place);

    QString political_state() const;
    void set_political_state(const QString &political_state);

    bool married() const;
    void set_married(bool married);

    QString nation() const;
    void set_nation(const QString &nation);

    int education() const;
    void set_education(int education);

    QString telephone() const;
    void set_telephone(const QString &telephone);

    QString email() const;
    void set_email(const QString &email);

    int state() const;
    void set_state(int state);

    bool is_new() const;
    void set_is_new(bool is_new);

    int access_level() const;
    void set_access_level(int access_level);

    QJsonObject to_json();

private:
    QString m_name;
    QString m_username;
    QString m_password;
    QString m_id;
    int m_gender;
    QString m_photo_path;
    QString m_work_place;
    QDate m_birthday;
    QString m_birth_place;
    QString m_political_state;
    bool m_married;
    QString m_nation;
    int m_edu;
    QString m_telephone;
    QString m_email;
    int m_state;
    int m_access_level;
    bool m_is_new;
};

#endif // EMPLOYEE_H
