#include "lane.h"

CLane::CLane()
{

}

CLane::CLane(int lane_no, bool is_etc):
    m_lane_no(lane_no), m_is_etc(is_etc)
{}

CLane::CLane(const CLane &other){
    if(this!=&other){
      this->m_lane_no = other.m_lane_no;
      this->m_is_etc = other.m_is_etc;
    }
}

CLane CLane::operator=(const CLane& other){
    if(this!=&other){
      this->m_lane_no = other.m_lane_no;
      this->m_is_etc = other.m_is_etc;
    }
    return *this;
}

CLane::CLane(const QJsonObject &obj){
    m_lane_no = obj.value("lane_no").toInt();
    m_is_etc = obj.value("is_etc").toBool();
}


QJsonObject CLane::toJSON() const{
    QJsonObject lane_json;
    lane_json.insert("lane_no", m_lane_no);
    lane_json.insert("is_etc", m_is_etc);
    return lane_json;
}
