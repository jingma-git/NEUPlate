#ifndef HIGHWAY_TOLL_H
#define HIGHWAY_TOLL_H

#include "link_toll.h"
#include <QString>

/**
* @struct        CHighWayToll
* @brief         the class that store the toll stattion information.
* @author        luxijia
* @date          2018-07-26
*/
class CHighWayToll
{
public:
    CHighWayToll();
    CHighWayToll(int index, const QString &toll_name);
    ~CHighWayToll();

    int index() const;
    void setIndex(int index);

    CLinkToll *head() const;
    void setHead(CLinkToll *head);

    QString toll_name() const;
    void setToll_name(const QString &toll_name);

private:
    int m_index;
    QString m_toll_name;
    CLinkToll *m_head;
};

#endif // HIGHWAY_TOLL_H
