#ifndef DEDUCTION_H
#define DEDUCTION_H

#include "Business/Deduction/CarTypeDeduction/car_type_deduction_head.h"

/**
* @name          CDeduction
* @brief         this class implements the calculate highway charge fee according to province where highway belong to.
*                It's a interface that will be repleaced by actually implement sub-class which is factory pattern.
* @author        luxijia
* @date          2018-07-28
*/
class CDeduction
{
public:
    virtual ~CDeduction();
    virtual double deduction(const QString &highway_id, const double distance) = 0;
    virtual void set_car_type(const QString &car_type) = 0;
protected:
    virtual double format_fee(double fee);
    CCarTypeDeduction *m_car_type_deduction;
};

#endif // DEDUCTION_H
