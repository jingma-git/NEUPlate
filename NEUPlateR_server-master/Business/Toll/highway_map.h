#ifndef HIGHWEY_MAP_H
#define HIGHWEY_MAP_H

#include "highway_toll.h"

/**
* @class         CHighWayMap
* @brief         this class inplements that the city highway map.
* @author        luxijia
* @date          2018-07-26
*/
class CHighWayMap
{
public:
    CHighWayMap(int toll_number,const QString &province_name);
    ~CHighWayMap();
    int find_toll_index(const QString &toll_name);
    void set_toll_attay(int index, const QString &toll_name);


    int toll_number() const;
    void setToll_number(int toll_number);

    QString province_name() const;
    void setProvince_name(const QString &province_name);

    CHighWayToll *toll_array() const;
    void setToll_array(CHighWayToll *toll_array);

private:
    int m_toll_number;
    QString m_province_name;
    CHighWayToll *m_toll_array;
};

#endif // HIGHWEY_MAP_H
