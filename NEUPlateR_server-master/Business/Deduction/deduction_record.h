#ifndef DEDUCTION_RECORD_H
#define DEDUCTION_RECORD_H

#include <QString>

/**
* @class         CDedutionRecord
* @brief         this class implements the the deduction record.
* @author        luxijia
* @date          2018-07-31
*/
class CDedutionRecord
{
public:
    QString deduction_id() const;
    void setDeduction_id(const QString &deduction_id);

    QString plate() const;
    void setPlate(const QString &plate);

    QString start_name() const;
    void setStart_name(const QString &start_name);

    QString end_name() const;
    void setEnd_name(const QString &end_name);

    double distance() const;
    void setDistance(double distance);

    double fee() const;
    void setFee(double fee);

    int pay_menthod() const;
    void setPay_menthod(int pay_menthod);

    QString deduction_time() const;
    void setDeduction_time(const QString &deduction_time);

private:
    QString m_deduction_id;
    QString m_plate;
    QString m_start_name;
    QString m_end_name;
    double m_distance;
    double m_fee;
    int m_pay_menthod;
    QString m_deduction_time;
};

#endif // DEDUCTION_RECORD_H
