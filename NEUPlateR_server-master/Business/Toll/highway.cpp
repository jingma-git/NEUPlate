#include "highway.h"

QString CHighWay::start() const
{
    return m_start;
}

void CHighWay::setStart(const QString &start)
{
    m_start = start;
}

QString CHighWay::end() const
{
    return m_end;
}

void CHighWay::setEnd(const QString &end)
{
    m_end = end;
}

QString CHighWay::id() const
{
    return m_id;
}

void CHighWay::setId(const QString &id)
{
    m_id = id;
}

double CHighWay::distance() const
{
    return m_distance;
}

void CHighWay::setDistance(double distance)
{
    m_distance = distance;
}
