#include "response.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QByteArray>

CResponse::CResponse()
{

}

CResponse::CResponse(const QJsonObject &json)
    :CBody(json["params"].toObject()),
      m_i_status_code(json["code"].toInt()),
      m_str_desc(json["desc"].toString()),
      m_action(json["action"].toString()),
      m_started_by(json["started_by"].toString())
{

}

int CResponse::status_code() const
{
    return m_i_status_code;
}

void CResponse::set_status_code(int i_status_code)
{
    m_i_status_code = i_status_code;
}

QString CResponse::desc() const
{
    return m_str_desc;
}

void CResponse::set_desc(const QString &str_desc)
{
    m_str_desc = str_desc;
}

QString CResponse::to_json() const
{
    QJsonObject json;
    json.insert(QString("code"), m_i_status_code);
    json.insert(QString("desc"), m_str_desc);
    json.insert(QString("params"), m_json_params);
    json.insert(QString("action"), m_action);
    json.insert(QString("started_by"), m_started_by);
    QJsonDocument json_doc(json);
    QByteArray json_data(json_doc.toJson(QJsonDocument::Compact));
    return QString(json_data);
}

QString CResponse::getAction() const
{
    return m_action;
}

void CResponse::set_action(const QString &action)
{
    m_action = action;
}

QString CResponse::getStarted_by() const
{
    return m_started_by;
}

void CResponse::set_started_by(const QString &started_by)
{
    m_started_by = started_by;
}
