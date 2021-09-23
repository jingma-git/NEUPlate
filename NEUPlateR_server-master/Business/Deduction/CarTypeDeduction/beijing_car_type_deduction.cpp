#include "beijing_car_type_deduction.h"

/**
* @name          ~CBeijingFirstClassCar
* @brief         class CBeijingFirstClassCar deconstructor that release class resource and
*                call super class deconstrutor.
* @author        luxijia
* @date          2018-07-28
*/
CBeijingFirstClassCar::~CBeijingFirstClassCar()
{

}

/**
* @name          calculate_fee
* @brief         calculate liao ning provine first class fee.
* @author        luxijia
* @date          2018-07-28
* @param         start_toll the start toll station
* @param         end_toll the end toll station
* @param         distance the dis
* @return
*/
double CBeijingFirstClassCar::calculate_fee(const QString &highway_id, const double distacne)
{
    CDeductionDAO deduction_dao;
    int code;
    int charge_type;
    double basic_price;
    double price;
    double fee;

    code = deduction_dao.query_charge_type(charge_type, "1", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get charge type failed.");
        return CALCULATE_FAILED;
    }

    code = deduction_dao.query_charge_price(price, "1", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get highway charge failed.");
        return CALCULATE_FAILED;
    }

    if (1 == charge_type)
        fee = distacne * price;
    else if (2 == charge_type)
        fee = price;

    code = deduction_dao.query_basic_price(basic_price, "1", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get highway basic price failed.");
        return CALCULATE_FAILED;
    }

    if (fee < basic_price)
        fee = basic_price;

    return fee;
}

/**
* @name          ~CBeijingSecondClassCar
* @brief         class CBeijingSecondClassCar deconstructor that release class resource and
*                call super class deconstrutor.
* @author        luxijia
* @date          2018-07-28
*/
CBeijingSecondClassCar::~CBeijingSecondClassCar()
{

}

/**
* @name          calculate_fee
* @brief         calculate liao ning provine second class fee.
* @author        luxijia
* @date          2018-07-28
* @param         start_toll the start toll station
* @param         end_toll the end toll station
* @param         distance the dis
* @return
*/
double CBeijingSecondClassCar::calculate_fee(const QString &highway_id, const double distacne)
{
    CDeductionDAO deduction_dao;
    int code;
    int charge_type;
    double basic_price;
    double price;
    double fee;

    code = deduction_dao.query_charge_type(charge_type, "2", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get charge type failed.");
        return CALCULATE_FAILED;
    }

    code = deduction_dao.query_charge_price(price, "2", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get highway charge failed.");
        return CALCULATE_FAILED;
    }

    if (1 == charge_type)
        fee = distacne * price;
    else if (2 == charge_type)
        fee = price;

    code = deduction_dao.query_basic_price(basic_price, "2", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get highway basic price failed.");
        return CALCULATE_FAILED;
    }

    if (fee < basic_price)
        fee = basic_price;

    return fee;
}

/**
* @name          ~CBeijingThirdClassCar
* @brief         class CBeijingThirdClassCar deconstructor that release class resource and
*                call super class deconstrutor.
* @author        luxijia
* @date          2018-07-28
*/
CBeijingThirdClassCar::~CBeijingThirdClassCar()
{

}

/**
* @name          calculate_fee
* @brief         calculate liao ning provine first class fee.
* @author        luxijia
* @date          2018-07-28
* @param         start_toll the start toll station
* @param         end_toll the end toll station
* @param         distance the dis
* @return
*/
double CBeijingThirdClassCar::calculate_fee(const QString &highway_id, const double distacne)
{
    CDeductionDAO deduction_dao;
    int code;
    int charge_type;
    double basic_price;
    double price;
    double fee;

    code = deduction_dao.query_charge_type(charge_type, "3", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get charge type failed.");
        return CALCULATE_FAILED;
    }

    code = deduction_dao.query_charge_price(price, "3", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get highway charge failed.");
        return CALCULATE_FAILED;
    }

    if (1 == charge_type)
        fee = distacne * price;
    else if (2 == charge_type)
        fee = price;

    code = deduction_dao.query_basic_price(basic_price, "3", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get highway basic price failed.");
        return CALCULATE_FAILED;
    }

    if (fee < basic_price)
        fee = basic_price;

    return fee;
}

/**
* @name          ~CBeijingFourthClassCar
* @brief         class CBeijingFourthClassCar deconstructor that release class resource and
*                call super class deconstrutor.
* @author        luxijia
* @date          2018-07-28
*/
CBeijingFourthClassCar::~CBeijingFourthClassCar()
{

}

/**
* @name          calculate_fee
* @brief         calculate liao ning provine first class fee.
* @author        luxijia
* @date          2018-07-28
* @param         start_toll the start toll station
* @param         end_toll the end toll station
* @param         distance the dis
* @return
*/
double CBeijingFourthClassCar::calculate_fee(const QString &highway_id, const double distacne)
{
    CDeductionDAO deduction_dao;
    int code;
    int charge_type;
    double basic_price;
    double price;
    double fee;

    code = deduction_dao.query_charge_type(charge_type, "4", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get charge type failed.");
        return CALCULATE_FAILED;
    }

    code = deduction_dao.query_charge_price(price, "4", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get highway charge failed.");
        return CALCULATE_FAILED;
    }

    if (1 == charge_type)
        fee = distacne * price;
    else if (2 == charge_type)
        fee = price;

    code = deduction_dao.query_basic_price(basic_price, "4", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get highway basic price failed.");
        return CALCULATE_FAILED;
    }

    if (fee < basic_price)
        fee = basic_price;

    return fee;
}

/**
* @name          ~CBeijingFifthClassCar
* @brief         class CBeijingFifthClassCar deconstructor that release class resource and
*                call super class deconstrutor.
* @author        luxijia
* @date          2018-07-28
*/
CBeijingFifthClassCar::~CBeijingFifthClassCar()
{

}

/**
* @name          calculate_fee
* @brief         calculate liao ning provine first class fee.
* @author        luxijia
* @date          2018-07-28
* @param         start_toll the start toll station
* @param         end_toll the end toll station
* @param         distance the dis
* @return
*/
double CBeijingFifthClassCar::calculate_fee(const QString &highway_id, const double distacne)
{
    CDeductionDAO deduction_dao;
    int code;
    int charge_type;
    double basic_price;
    double price;
    double fee;

    code = deduction_dao.query_charge_type(charge_type, "5", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get charge type failed.");
        return CALCULATE_FAILED;
    }

    code = deduction_dao.query_charge_price(price, "5", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get highway charge failed.");
        return CALCULATE_FAILED;
    }

    if (1 == charge_type)
        fee = distacne * price;
    else if (2 == charge_type)
        fee = price;

    code = deduction_dao.query_basic_price(basic_price, "5", highway_id);

    if (StatusCode::SUCCESS != code)
    {
        Log::warning("Get highway basic price failed.");
        return CALCULATE_FAILED;
    }

    if (fee < basic_price)
        fee = basic_price;

    return fee;
}
