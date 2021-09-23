#ifndef LIAONING_CAR_TYPE_DEDUCTION_H
#define LIAONING_CAR_TYPE_DEDUCTION_H

#include "car_type_deduction.h"

/**
* @name          CLiaoNingFirstClassCar
* @brief         this class inherit class CCarTypeDeduction that implements calculate
*                Liao Niao province first class car fee.
* @author        luxijia
* @date          2018-07-28
*/
class CLiaoNingFirstClassCar : public CCarTypeDeduction
{
public:
    ~CLiaoNingFirstClassCar();
    double calculate_fee(const QString &highway_id, const double distacne);
};

/**
* @name          CLiaoNingSecondClassCar
* @brief         this class inherit class CCarTypeDeduction that implements calculate
*                Liao Niao province second class car fee.
* @author        luxijia
* @date          2018-07-28
*/
class CLiaoNingSecondClassCar : public CCarTypeDeduction
{
public:
    ~CLiaoNingSecondClassCar();
    double calculate_fee(const QString &highway_id, const double distacne);
};

/**
* @name          CLiaoNingThirdClassCar
* @brief         this class inherit class CCarTypeDeduction that implements calculate
*                Liao Niao province third class car fee.
* @author        luxijia
* @date          2018-07-28
*/
class CLiaoNingThirdClassCar : public CCarTypeDeduction
{
public:
    ~CLiaoNingThirdClassCar();
    double calculate_fee(const QString &highway_id, const double distacne);
};

/**
* @name          CLiaoNingFourthClassCar
* @brief         this class inherit class CCarTypeDeduction that implements calculate
*                Liao Niao province fourth class car fee.
* @author        luxijia
* @date          2018-07-28
*/
class CLiaoNingFourthClassCar : public CCarTypeDeduction
{
public:
    ~CLiaoNingFourthClassCar();
    double calculate_fee(const QString &highway_id, const double distacne);
};

/**
* @name          CLiaoNingFifthClassCar
* @brief         this class inherit class CCarTypeDeduction that implements calculate
*                Liao Niao province fifth class car fee.
* @author        luxijia
* @date          2018-07-28
*/
class CLiaoNingFifthClassCar : public CCarTypeDeduction
{
public:
    ~CLiaoNingFifthClassCar();
    double calculate_fee(const QString &highway_id, const double distacne);
};

#endif // LIAONING_CAR_TYPE_DEDUCTION_H
