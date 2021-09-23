#include "toll_station.h"

CTollStation::CTollStation()
{

}

CTollStation::CTollStation(const QString &id, const QString &name, QString &province, std::vector<CLane>& lanes):
    m_id(id),m_name(name),m_province(province),m_lanes(lanes){}

CTollStation::CTollStation(QJsonObject &obj){
    m_id = obj.value("id").toString();
    m_name = obj.value("name").toString();
    m_province = obj.value("province").toString();
    QJsonArray lane_array = obj.value("lanes").toArray();
    for(const QJsonValue &val: lane_array){
        CLane lane(val.toObject());
        m_lanes.push_back(lane);
    }
}

QJsonObject CTollStation::toJSON() const{
    QJsonObject station_json;
    station_json.insert("id", m_id);
    station_json.insert("name",m_name);
    station_json.insert("province", m_province);
    QJsonArray lane_array;
    int size = m_lanes.size();
    for(int i=0; i<size; i++){
        lane_array.append(m_lanes[i].toJSON());
    }
    station_json.insert("lanes", lane_array);
    return station_json;
}
QString CTollStation::id() const{
    return this->m_id;
}
void CTollStation::setId(const QString &id){
    this->m_id = id;
}

QString CTollStation::name() const{
    return this->m_name;
}
void CTollStation::setName(const QString &name){
    this->m_name = name;
}

QString CTollStation::province() const{
    return this->m_province;
}
void CTollStation::setProvince(const QString &province){
    this->m_province = province;
}

std::vector<CLane> CTollStation::lanes() const{
    return this->m_lanes;
}
void CTollStation::setLanes(const std::vector<CLane> &lanes){
    this->m_lanes = lanes;
}
