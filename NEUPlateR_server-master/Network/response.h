#ifndef RESPONSE_H
#define RESPONSE_H

#include "body.h"

class CResponse : public CBody
{
public:
    CResponse();
    CResponse(const QJsonObject &json);

    int status_code() const;
    void set_status_code(int status_code);

    QString desc() const;
    void set_desc(const QString &desc);

    QString to_json() const;

    QString getAction() const;
    void set_action(const QString &action);

    QString getStarted_by() const;
    void set_started_by(const QString &started_by);

private:
    int m_i_status_code;
    QString m_str_desc;
    QString m_action;
    QString m_started_by;
};

#endif // RESPONSE_H
