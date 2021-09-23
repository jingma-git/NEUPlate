#ifndef CAR_TYPE_DEDUCTION_H
#define CAR_TYPE_DEDUCTION_H

#include <QString>
#include "Business/Deduction/deduction_dao.h"
#include "neu_head.h"
#define CALCULATE_FAILED -1

/**
* @name          CCarTypeDeduction
* @brief         this class implements the calculate car fee by car type, distance, dirve road type
*                and dirve road charge standard.
*                It's a interface that will be repleaced by actually implement sub-class which is strategy pattern.
* @author        luxijia
* @date          2018-07-28
*/
class CCarTypeDeduction
{
public:
    virtual ~CCarTypeDeduction();
    virtual double calculate_fee(const QString &highway_id, const double distacne) = 0;
};

#endif // CAR_TYPE_DEDUCTION_H
