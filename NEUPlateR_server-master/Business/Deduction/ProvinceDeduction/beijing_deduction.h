#ifndef BEIJING_DEDUCTION_H
#define BEIJING_DEDUCTION_H

#include "deduction.h"

/**
* @name          CBeijingDeduction
* @brief         this class inhetit class CDeduction that implements beijing municapality highway Deduction.
* @author        luxijia
* @date          2018-07-29
*/
class CBeijingDeduction : public CDeduction
{
public:
    ~CBeijingDeduction();
    double deduction(const QString &highway_id, const double distance);
    void set_car_type(const QString &car_type);
};

#endif // BEIJING_DEDUCTION_H
