#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <QString>

/**
* @class         CHighWayCHighWayap
* @brief         this class inplements that store the highway information.
* @author        luxijia
* @date          2018-07-26
*/
class CHighWay
{
public:
    QString start() const;
    void setStart(const QString &start);

    QString end() const;
    void setEnd(const QString &end);

    QString id() const;
    void setId(const QString &id);

    double distance() const;
    void setDistance(double distance);

private:
    QString m_id;
    QString m_start;
    QString m_end;
    double m_distance;
};

#endif // HIGHWAY_H
