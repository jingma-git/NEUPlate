#include "highway_toll.h"
#include <iostream>
/**
* @name          CHighWayToll
* @brief         class CHighWayToll constructor without parameter.
* @author        luxijia
* @date          2018-07-26
*/
CHighWayToll::CHighWayToll():m_index(-1),m_toll_name(""),m_head(new CLinkToll())
{

}

/**
* @name          CHighWayToll
* @brief         class CHighWayToll constructor that initial the m_index, m_toll_name
* @author        luxijia
* @date          2018-07-26
*/
CHighWayToll::CHighWayToll(int index,const QString &toll_name):m_index(index),m_toll_name(toll_name),m_head(new CLinkToll())
{

}

/**
* @name          CHighWayToll
* @brief         class CHighWayToll deconstructor that delete m_head that is link toll.
* @author        luxijia
* @date          2018-07-26
*/
CHighWayToll::~CHighWayToll()
{   //if CLinkToll deconstructor also delete point
    //it will delete twice
    if (m_head != NULL)
    {
        while (m_head->next() != NULL)
        {
            CLinkToll *temp = m_head->next();
            m_head->setNext(temp->next());
            delete temp;
        }

        delete m_head;
        m_head = NULL;
    }
}

int CHighWayToll::index() const
{
    return m_index;
}

void CHighWayToll::setIndex(int index)
{
    m_index = index;
}

CLinkToll *CHighWayToll::head() const
{
    return m_head;
}

void CHighWayToll::setHead(CLinkToll *head)
{
    m_head = head;
}

QString CHighWayToll::toll_name() const
{
    return m_toll_name;
}

void CHighWayToll::setToll_name(const QString &toll_name)
{
    m_toll_name = toll_name;
}
