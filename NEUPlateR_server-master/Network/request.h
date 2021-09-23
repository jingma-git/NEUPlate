#ifndef REQUEST_H
#define REQUEST_H

#include "body.h"

class CRequest : public CBody
{
public:
    CRequest();
    CRequest(const QJsonObject &json);
    CRequest(const QString &req_str);

    QString to_json() const;

    QString action() const;
    void set_action(const QString &action);

    QString started_by() const;
    void set_started_by(const QString &started_by);

private:
    QString m_action;
    QString m_started_by;
};

#endif // REQUEST_H
