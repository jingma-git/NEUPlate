#ifndef LINK_TOLL_H
#define LINK_TOLL_H

#include <stdlib.h>

/**
* @class         CLinkToll
* @brief         this class that store one toll station link toll stations in highway map.
* @author        luxijia
* @date          2018-07-26
*/
class CLinkToll
{
public:
    CLinkToll();
    CLinkToll(int index, double distance);
    ~CLinkToll();
    int index() const;
    void setIndex(int index);

    double distance() const;
    void setDistance(double distance);

    CLinkToll *next() const;
    void setNext(CLinkToll *next);

private:
    int m_index;
    double m_distance;
    CLinkToll *m_next;
};

#endif // LINK_TOLL_H
