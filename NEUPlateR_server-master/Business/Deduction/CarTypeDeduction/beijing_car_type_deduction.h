#ifndef BEIJING_CAR_TYPE_DEDUCTION_H
#define BEIJING_CAR_TYPE_DEDUCTION_H
#include "car_type_deduction.h"

/**
* @name          CBeijingFirstClassCar
* @brief         this class inherit class CCarTypeDeduction that implements calculate
*                beijing  municipality first class car fee.
* @author        luxijia
* @date          2018-07-28
*/
class CBeijingFirstClassCar : public CCarTypeDeduction
{
public:
    ~CBeijingFirstClassCar();
    double calculate_fee(const QString &highway_id, const double distacne);
};

/**
* @name          CBeijingSecondClassCar
* @brief         this class inherit class CCarTypeDeduction that implements calculate
*                beijing  municipality second class car fee.
* @author        luxijia
* @date          2018-07-28
*/
class CBeijingSecondClassCar : public CCarTypeDeduction
{
public:
    ~CBeijingSecondClassCar();
    double calculate_fee(const QString &highway_id, const double distacne);
};

/**
* @name          CBeijingThirdClassCar
* @brief         this class inherit class CCarTypeDeduction that implements calculate
*                beijing  municipality third class car fee.
* @author        luxijia
* @date          2018-07-28
*/
class CBeijingThirdClassCar : public CCarTypeDeduction
{
public:
    ~CBeijingThirdClassCar();
    double calculate_fee(const QString &highway_id, const double distacne);
};

/**
* @name          CBeijingFourthClassCar
* @brief         this class inherit class CCarTypeDeduction that implements calculate
*                beijing  municipality fourth class car fee.
* @author        luxijia
* @date          2018-07-28
*/
class CBeijingFourthClassCar : public CCarTypeDeduction
{
public:
    ~CBeijingFourthClassCar();
    double calculate_fee(const QString &highway_id, const double distacne);
};

/**
* @name          CBeijingFifthClassCar
* @brief         this class inherit class CCarTypeDeduction that implements calculate
*                beijing  municipality fifth class car fee.
* @author        luxijia
* @date          2018-07-28
*/
class CBeijingFifthClassCar : public CCarTypeDeduction
{
public:
    ~CBeijingFifthClassCar();
    double calculate_fee(const QString &highway_id, const double distacne);
};

#endif // BEIJING_CAR_TYPE_DEDUTINO_H
