#include "liaoning_deduction.h"

/**
* @name          ~CLiaoNingDeduction
* @brief         class CLiaoNingDeduction deconstuctor that delete m_cat_type_Deduction point
*                and release class resource.
* @author        luxijia
* @date          2018-07-28
*/
CLiaoNingDeduction::~CLiaoNingDeduction()
{
    if (m_car_type_deduction != NULL)
    {
        delete m_car_type_deduction;
        m_car_type_deduction = NULL;
    }
}

/**
* @name          set_car_type
* @brief         set car type and determine use strategy.
* @author        luxijia
* @date          2018-07-28
* @param         car_type the type of car.
*/
void CLiaoNingDeduction::set_car_type(const QString &car_type)
{
   if ("1" == car_type)
       m_car_type_deduction = new CLiaoNingFirstClassCar();
   else if ("2" == car_type)
       m_car_type_deduction = new CLiaoNingSecondClassCar();
   else if ("3" == car_type)
       m_car_type_deduction = new CLiaoNingThirdClassCar();
   else if ("4" == car_type)
       m_car_type_deduction = new CLiaoNingFourthClassCar();
   else if ("5" == car_type)
       m_car_type_deduction = new CLiaoNingFifthClassCar();
}

/**
* @name          Deduction
* @brief         calulate car detution.
* @author        luxijia
* @date          2018-07-28
* @param         highway_id the id of highway
* @param         distance drive distance
* @return        if calculate success, return Deduction fee
*                or return -1;
*/
double CLiaoNingDeduction::deduction(const QString &highway_id, const double distance)
{
    double fee;
    fee = m_car_type_deduction->calculate_fee(highway_id, distance);

    if (CALCULATE_FAILED == fee)
        return CALCULATE_FAILED;

    fee = format_fee(fee);
    return fee;
}
