#ifndef BODY_H
#define BODY_H
#include <QJsonObject>

class CBody
{
public:
    CBody(const QJsonObject &json);
    CBody();
    virtual ~CBody();
    void set_params(const QJsonObject &param);

    // put function
    void put(const QString &key, int value);
    void put(const QString &key, double value);
    void put(const QString &key, bool value);
    void put(const QString &key, const QString &value);
    void put(const QString &key, const char *value);
    void put(const QString &key, const QJsonObject &json);
    void put(const QString &key, const QJsonArray &array);

    // get function
    int get_int(const QString &key) const;
    double get_double(const QString &key) const;
    QString get_string(const QString &key) const;
    bool get_bool(const QString &key) const;
    QJsonObject get_json(const QString &key) const;
    QJsonArray get_array(const QString &key) const;

protected:
    QJsonObject m_json_params;
};

#endif // BODY_H
