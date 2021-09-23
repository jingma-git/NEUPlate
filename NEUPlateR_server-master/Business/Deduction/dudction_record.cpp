#include "deduction_record.h"

QString CDedutionRecord::deduction_id() const
{
    return m_deduction_id;
}

void CDedutionRecord::setDeduction_id(const QString &deduction_id)
{
    m_deduction_id = deduction_id;
}

QString CDedutionRecord::plate() const
{
    return m_plate;
}

void CDedutionRecord::setPlate(const QString &plate)
{
    m_plate = plate;
}

QString CDedutionRecord::start_name() const
{
    return m_start_name;
}

void CDedutionRecord::setStart_name(const QString &start_name)
{
    m_start_name = start_name;
}

QString CDedutionRecord::end_name() const
{
    return m_end_name;
}

void CDedutionRecord::setEnd_name(const QString &end_name)
{
    m_end_name = end_name;
}

double CDedutionRecord::distance() const
{
    return m_distance;
}

void CDedutionRecord::setDistance(double distance)
{
    m_distance = distance;
}

double CDedutionRecord::fee() const
{
    return m_fee;
}

void CDedutionRecord::setFee(double fee)
{
    m_fee = fee;
}

int CDedutionRecord::pay_menthod() const
{
    return m_pay_menthod;
}

void CDedutionRecord::setPay_menthod(int pay_menthod)
{
    m_pay_menthod = pay_menthod;
}

QString CDedutionRecord::deduction_time() const
{
    return m_deduction_time;
}

void CDedutionRecord::setDeduction_time(const QString &deduction_time)
{
    m_deduction_time = deduction_time;
}
