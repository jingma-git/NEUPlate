#include "highway_map.h"

/**
* @name          CHighWayMap
* @brief         class CHighWayMap constructor that initial the m_toll_number, m_province_city
* @author        luxijia
* @date          2018-07-26
*/
CHighWayMap::CHighWayMap(int toll_number,const QString &province_name):m_toll_number(toll_number),m_province_name(province_name),m_toll_array(NULL)
{

}

/**
* @name          CHighWayMap
* @brief         class CHighWayMap deconstructor that delete m_toll_array and release clss resource.
* @author        luxijia
* @date          2018-07-26
*/
CHighWayMap::~CHighWayMap()
{
    delete [] m_toll_array;
}

/**
* @name          find_toll_index
* @brief         find toll station index in array.
* @author        luxijia
* @date          2018-07-26
* @param         roll_name the name/id of toll station.
* @return        the index in array
*                or -1 if no find.
*/
int CHighWayMap::find_toll_index(const QString &toll_name)
{
    for (int i = 0; i < m_toll_number; i++)
    {
        if (toll_name == m_toll_array[i].toll_name())
        {
            return i;
        }
    }

    return -1;
}

void CHighWayMap::set_toll_attay(int index, const QString &toll_name)
{
    m_toll_array[index].setIndex(index);
    m_toll_array[index].setToll_name(toll_name);
}

int CHighWayMap::toll_number() const
{
    return m_toll_number;
}

void CHighWayMap::setToll_number(int toll_number)
{
    m_toll_number = toll_number;
}

QString CHighWayMap::province_name() const
{
    return m_province_name;
}

void CHighWayMap::setProvince_name(const QString &province_name)
{
    m_province_name = province_name;
}

CHighWayToll *CHighWayMap::toll_array() const
{
    return m_toll_array;
}

void CHighWayMap::setToll_array(CHighWayToll *toll_array)
{
    m_toll_array = toll_array;
}
