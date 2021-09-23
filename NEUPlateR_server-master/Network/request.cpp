#include "request.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QByteArray>

CRequest::CRequest()
{

}

CRequest::CRequest(const QJsonObject &json)
    :CBody(json["params"].toObject()), m_action(json["action"].toString()),m_started_by(json["started_by"].toString())
{

}

/**
* @functionName  CRequest
* @brief         consturct CRequest object from request string data
* @author        chenhanlin
* @date          2018-07-25
* @parameter     void
* @return        void
*/
CRequest::CRequest(const QString &req_str)
{
    QByteArray data = req_str.toUtf8();
    QJsonDocument json_doc(QJsonDocument::fromJson(data));
    QJsonObject json(json_doc.object());
    this->set_action(json["action"].toString());
    this->set_params(json["params"].toObject());
    this->set_started_by(json["started_by"].toString());
}

QString CRequest::action() const
{
    return m_action;
}

void CRequest::set_action(const QString &action)
{
    m_action = action;
}

QString CRequest::started_by() const
{
    return m_started_by;
}

void CRequest::set_started_by(const QString &started_by)
{
    m_started_by = started_by;
}


/**
* @functionName  transfer_to_json
* @Description   transfer request'data to json
* @author        chenhanlin
* @date          2018-07-03
* @parameter     void
* @return        QString
*/
QString CRequest::to_json() const
{
    QJsonObject json;
    json.insert(QString("action"), m_action);
    json.insert(QString("params"), m_json_params);
    json.insert(QString("started_by"), m_started_by);
    QJsonDocument json_doc(json);
    QByteArray json_data(json_doc.toJson(QJsonDocument::Compact));
    return QString(json_data);
}
