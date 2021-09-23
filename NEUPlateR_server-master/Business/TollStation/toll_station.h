#ifndef TOLL_STATION_H
#define TOLL_STATION_H


/**
* @projectName   CTollStation
* @brief         收费站实体类
* @author        majing
* @date          2018-07-29
* @modify_author XXXX
* @modify_date   2018-07-29
*/

#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <vector>
#include "lane.h"

class CTollStation
{
public:
    CTollStation();
    CTollStation(const QString &id, const QString &name, QString &province, std::vector<CLane>& lanes);
    CTollStation(QJsonObject &obj);
    QJsonObject toJSON() const;

    QString id() const;
    void setId(const QString &id);

    QString name() const;
    void setName(const QString &name);

    QString province() const;
    void setProvince(const QString &province);

    std::vector<CLane> lanes() const;
    void setLanes(const std::vector<CLane> &lanes);

    const static int IS_ENTRY = 0;
    const static int NOT_ENTRY = 1;

private:
    QString m_id;
    QString m_name;
    QString m_province;
    std::vector<CLane> m_lanes;
};

#endif // TOLL_STATION_H
