#ifndef RECOG_RECORD_H
#define RECOG_RECORD_H

#include <QString>
#include <QJsonObject>
/**
 * @brief The CRecogRecord class 进出站记录-
 */
class CRecogRecord
{
public:
    static const int ENTER_STATATION;
    static const int EXIT_STATION;

    CRecogRecord(const QString &record_id,
                 const QString &plate_no,
                 const QString &date_time,
                 const QString &toll_station,
                 const QString &photo_path,
                 const int &lane_no,
                 const int &type,
                 const bool &is_etc);
    CRecogRecord();
    CRecogRecord(QJsonObject json_obj);

    QString record_id() const;
    void setRecordId(const QString &record_id);

    QString plate_no() const;
    void setPlateNo(const QString &plate_no);

    QString date_time() const;
    void setDateTime(const QString &date_time);

    QString toll_station() const;
    void setTollStation(const QString &toll_station);

    QString photo_path() const;
    void setPhotoPath(const QString &photo_path);

    int lane_no() const;
    void setLaneNo(const int &lane_no);

    int type() const;
    void setType(const int &type);

    bool is_etc() const;
    bool setIsEtc(const bool &is_etc);

private:
    QString m_record_id;
    QString m_plate_no;
    QString m_date_time;
    QString m_toll_station;
    QString m_photo_path;
    int m_lane_no;
    int m_type; //进站type=0,出站type=1
    bool m_is_etc;
};

#endif // RECOG_RECORD_H
