#ifndef LIAONING_DEDUCTION_H
#define LIAONING_DEDUCTION_H

#include "deduction.h"

/**
* @name          CLiaoNingDeduction
* @brief         this class inhetit class CDeduction that implements liao ning province highway Deduction.
* @author        luxijia
* @date          2018-07-28
*/
class CLiaoNingDeduction : public CDeduction
{
public:
    ~CLiaoNingDeduction();
    double deduction(const QString &highway_id, const double distance);
    void set_car_type(const QString &car_type);
};

#endif // LIAONING_DEDUCTION_H
