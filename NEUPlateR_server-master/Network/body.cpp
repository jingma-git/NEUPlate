#include "body.h"
#include <QJsonArray>
#include "Exception/null_exception.h"

CBody::CBody(const QJsonObject &json):m_json_params(json)
{

}

CBody::CBody()
{

}

CBody::~CBody()
{

}

void CBody::set_params(const QJsonObject &param)
{
    this->m_json_params = param;
}

/**
* @functionName  put
* @Description   put the int value in key, if not success, params will not have this value
* @author        chenhanlin
* @date          2018-06-28
* @parameter     QString key, int value
* @return        void
*/
void CBody::put(const QString &key, int value)
{
    this->m_json_params.insert(key, value);
}
 /**
* @functionName  put
* @Description   put the double value in key, if not success, params will not have this value
* @author        chenhanlin
* @date          2018-06-28
* @parameter     QString key, double value
* @return        void
*/
void CBody::put(const QString &key, double value)
{
    this->m_json_params.insert(key, value);
}

 /**
* @functionName  put
* @Description   put the bool value in key, if not success, params will not have this value
* @author        chenhanlin
* @date          2018-06-28
* @parameter     QString key, bool value
* @return        void
*/
void CBody::put(const QString &key, bool value)
{
    this->m_json_params.insert(key, value);
}

/**
* @functionName  put
* @Description   put the string value in key, if not success, params will not have this value
* @author        chenhanlin
* @date          2018-06-28
* @parameter     QString &key, QString &value
* @return        void
*/
void CBody::put(const QString &key, const QString &value)
{
    this->m_json_params.insert(key, value);
}

/**
* @functionName  put
* @Description   put the string value in key, if not success, params will not have this value
* @author        chenhanlin
* @date          2018-06-28
* @parameter     QString &key, QString &value
* @return        void
*/
void CBody::put(const QString &key, const char *value)
{
    this->m_json_params.insert(key, value);
}

/**
* @functionName  put
* @Description   put the json object in key, if not success, params will not have this value
* @author        chenhanlin
* @date          2018-06-28
* @parameter     QString key, QJsonObejct value
* @return        void
*/
void CBody::put(const QString &key, const QJsonObject &value)
{
    this->m_json_params.insert(key, value);
}

/**
* @functionName  put
* @Description   put the json array in key, if not success, params will not have this value
* @author        chenhanlin
* @date          2018-06-28
* @parameter     QString key, QJsonArray value
* @return        void
*/
void CBody::put(const QString &key, const QJsonArray &value)
{
    this->m_json_params.insert(key, value);
}

/**
* @functionName  get_int
* @Description   get the value by key from m_json_params, and return type is int
* @author        chenhanlin
* @date          2018-06-28
* @parameter     QString key
* @return        int
*/
int CBody::get_int(const QString &key) const
{
    if(!m_json_params.contains(key)){
        throw NullException();
    }else{
        return m_json_params[key].toInt();
    }
}

/**
* @functionName  get_double
* @Description   get the value by key from m_json_params and return type is double
* @author        chenhanlin
* @date          2018-06-28
* @parameter     QString key
* @return        double
*/
double CBody::get_double(const QString &key) const
{
    if (!m_json_params.contains(key)){
        throw NullException();
    }else{
        return m_json_params[key].toDouble();
    }
}

/**
* @functionName  get_string
* @Description   get the value by key from m_json_params and return type is std::string
* @author        chenhanlin
* @date          2018-06-28
* @parameter     QString key
* @return        QString
*/
QString CBody::get_string(const QString &key) const
{
    if (!m_json_params.contains(key)){
        throw NullException();
    }else{
        return m_json_params[key].toString();
    }
}

/**
* @functionName  get_bool
* @Description   get the value by key from m_json_params and return type is bool
* @author        chenhanlin
* @date          2018-06-28
* @parameter     QString key
* @return        bool
*/
bool CBody::get_bool(const QString &key) const
{
    if (!m_json_params.contains(key)){
        throw NullException();
    }else{
        return m_json_params[key].toBool();
    }
}

/**
* @functionName  get_json
* @Description   get the value by key from m_json_params and return type is QJsonObejct
* @author        chenhanlin
* @date          2018-06-28
* @parameter     QString key
* @return        QJsonObeject
*/
QJsonObject CBody::get_json(const QString &key) const
{
    if (!m_json_params.contains(key)){
        throw NullException();
    }else{
        return m_json_params[key].toObject();
    }
}

/**
* @functionName  get_array
* @Description   get the value by key from m_json_params and return type is QJsonArray
* @author        chenhanlin
* @date          2018-06-28
* @parameter     QString key
* @return        QJsonArrar
*/
QJsonArray CBody::get_array(const QString &key) const
{
    if (!m_json_params.contains(key)){
        throw NullException();
    }else{
        return m_json_params[key].toArray();
    }
}
