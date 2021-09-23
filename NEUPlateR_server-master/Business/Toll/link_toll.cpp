#include "link_toll.h"
#include <QDebug>

/**
* @name          CLinkToll
* @brief         class CLinkToll constructor without parameter.
* @author        luxijia
* @date          2018-07-26
*/
CLinkToll::CLinkToll():m_index(0),m_distance(0),m_next(NULL)
{

}

/**
* @name          CLinkToll
* @brief         class CLinkToll constructor that initial the m_index, m_distance
* @author        luxijia
* @date          2018-07-26
*/
CLinkToll::CLinkToll(int index, double distance):m_index(index),m_distance(distance),m_next(NULL)
{

}

/**
* @name          CLinkToll
* @brief         class CLinkToll deconstructor that delete m_next and release class resource.
* @author        luxijia
* @date          2018-07-26
*/
CLinkToll::~CLinkToll()
{

}

int CLinkToll::index() const
{
    return m_index;
}

void CLinkToll::setIndex(int index)
{
    m_index = index;
}

double CLinkToll::distance() const
{
    return m_distance;
}

void CLinkToll::setDistance(double distance)
{
    m_distance = distance;
}

CLinkToll *CLinkToll::next() const
{
    return m_next;
}

void CLinkToll::setNext(CLinkToll *next)
{
    m_next = next;
}
