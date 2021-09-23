#include "deduction.h"

/**
* @name          ~CDeduction
* @brief         class CDeduction deconstuctor that delete m_cat_type_Deduction point
*                and release class resource.
* @author        luxijia
* @date          2018-07-28
*/
CDeduction::~CDeduction()
{
    if (m_car_type_deduction != NULL)
    {
        delete m_car_type_deduction;
        m_car_type_deduction = NULL;
    }
}

/**
* @name          format_fee
* @brief         format fee that according to highway "Give two to eight, three to seven to five".
* @author        luxijia
* @date          2018-07-28
* @param         highway_id the id of highway
* @param         distance drive distance
* @return        if calculate success, return Deduction fee
*                or return -1;
*/
double CDeduction::format_fee(double fee)
{
    int fee_front;
    int fee_last;
    int integer_fee;
    double fee_result;

    integer_fee = floor(fee);

    fee_last = integer_fee % 10;
    fee_front = integer_fee / 10;

    if (fee_last < 3)
        fee_result = fee_front * 10;
    else if (fee_last >= 3 && fee_last <= 7)
        fee_result = fee_front * 10 + 5;
    else if (fee_last > 7)
        fee_result = (fee_front + 1) * 10;

    return fee_result;
}
