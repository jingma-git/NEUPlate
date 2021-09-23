/**
* @projectName   CRecogRecord
* @brief         brief
* @author        majing
* @date          2018-07-25
* @modify_author XXXX
* @modify_date   2018-07-25
*/

#include "recog_record.h"

const int CRecogRecord::ENTER_STATATION=0; //上高速
const int CRecogRecord::EXIT_STATION=1; //下高速

CRecogRecord::CRecogRecord()
{

}

CRecogRecord::CRecogRecord(const QString &record_id,
                           const QString &plate_no,
                           const QString &date_time,
                           const QString &toll_station,
                           const QString &photo_path,
                           const int &lane_no,
                           const int &type,
                           const bool &is_etc):
    m_record_id(record_id),
    m_plate_no(plate_no),m_date_time(date_time),m_toll_station(toll_station),
    m_photo_path(photo_path),m_lane_no(lane_no),m_type(type), m_is_etc(is_etc){

}

CRecogRecord::CRecogRecord(QJsonObject json_obj){
    m_record_id = json_obj.value("record_id").toString();
    m_plate_no = json_obj.value("plate_no").toString();
    m_date_time = json_obj.value("date_time").toString();
    m_toll_station = json_obj.value("toll_station").toString();
    m_photo_path = json_obj.value("m_photo_path").toString();
    m_lane_no = json_obj.value("lane_no").toInt();
    m_type = json_obj.value("type").toInt();
    m_is_etc = json_obj.value("is_etc").toBool();
}

QString CRecogRecord::record_id() const{
    return m_record_id;
}
void CRecogRecord::setRecordId(const QString &record_id){
    m_record_id = record_id;
}

QString CRecogRecord::plate_no() const{
    return m_plate_no;
}
void CRecogRecord::setPlateNo(const QString &plate_no){
    m_plate_no = plate_no;
}

QString CRecogRecord::date_time() const{
    return m_date_time;
}
void CRecogRecord::setDateTime(const QString &date_time){
    m_date_time = date_time;
}

QString CRecogRecord::toll_station() const{
    return m_toll_station;
}
void CRecogRecord::setTollStation(const QString &toll_station){
    m_toll_station = toll_station;
}

QString CRecogRecord::photo_path() const{
    return m_photo_path;
}
void CRecogRecord::setPhotoPath(const QString &photo_path){
    m_photo_path = photo_path;
}

int CRecogRecord::lane_no() const{
    return m_lane_no;
}
void CRecogRecord::setLaneNo(const int &lane_no){
    m_lane_no = lane_no;
}

int CRecogRecord::type() const{
    return m_type;
}
void CRecogRecord::setType(const int &type){
    m_type = type;
}

bool CRecogRecord::is_etc() const{
    return m_is_etc;
}
bool CRecogRecord::setIsEtc(const bool &is_etc){
    m_is_etc = is_etc;
}
