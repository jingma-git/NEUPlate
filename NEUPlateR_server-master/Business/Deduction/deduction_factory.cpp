#include "deduction_factory.h"

/**
* @name          create_deduction
* @brief         Get calculate rule according to different provinces.
* @author        luxijia
* @date          2018-07-27
*/
CDeduction *CDeductionFactory::create_deduction(const QString &province)
{
    if ("liaoning" == province)
        return new CLiaoNingDeduction();

    if ("beijing" == province)
        return new CBeijingDeduction();

    return NULL;
}
