#ifndef DEDUCTION_FACTORY_H
#define DEDUCTION_FACTORY_H

#include "ProvinceDeduction/deduction_head.h"
#include <QString>
/**
* @class         CHighWayMap
* @brief         this class inplements that the city highway map.
* @author        luxijia
* @date          2018-07-26
*/
class CDeductionFactory
{
public:
    CDeduction *create_deduction(const QString &location);
};

#endif // DEDUCTION_FACTORY_H
