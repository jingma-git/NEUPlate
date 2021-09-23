#ifndef LANE_H
#define LANE_H

#include <QJsonObject>
class CLane
{
public:
    CLane();
    CLane(int lane_no, bool is_etc);
    CLane(const CLane& other);
    CLane operator=(const CLane& other);
    CLane(const QJsonObject &obj);
    QJsonObject toJSON() const;

    int m_lane_no;
    bool m_is_etc;
};

#endif // LANE_H
