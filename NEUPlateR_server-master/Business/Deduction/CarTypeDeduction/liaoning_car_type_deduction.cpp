#include "liaoning_car_type_deduction.h"

/**
* @name          ~CLiaoNingFirstClassCar
* @brief         class CLiaoNingFirstClassCar deconstructor that release class resource and
*                call super class deconstrutor.
* @author        luxijia
* @date          2018-07-28
*/
CLiaoNingFirstClassCar::~CLiaoNingFirstClassCar()
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
double CLiaoNingFirstClassCar::calculate_fee(const QString &highway_id, const double distacne)
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
* @name          ~CLiaoNingSecondClassCar
* @brief         class CLiaoNingSecondClassCar deconstructor that release class resource and
*                call super class deconstrutor.
* @author        luxijia
* @date          2018-07-28
*/
CLiaoNingSecondClassCar::~CLiaoNingSecondClassCar()
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
double CLiaoNingSecondClassCar::calculate_fee(const QString &highway_id, const double distacne)
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
* @name          ~CLiaoNingThirdClassCar
* @brief         class CLiaoNingThirdClassCar deconstructor that release class resource and
*                call super class deconstrutor.
* @author        luxijia
* @date          2018-07-28
*/
CLiaoNingThirdClassCar::~CLiaoNingThirdClassCar()
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
double CLiaoNingThirdClassCar::calculate_fee(const QString &highway_id, const double distacne)
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
* @name          ~CLiaoNingFourthClassCar
* @brief         class CLiaoNingFourthClassCar deconstructor that release class resource and
*                call super class deconstrutor.
* @author        luxijia
* @date          2018-07-28
*/
CLiaoNingFourthClassCar::~CLiaoNingFourthClassCar()
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
double CLiaoNingFourthClassCar::calculate_fee(const QString &highway_id, const double distacne)
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
* @name          ~CLiaoNingFifthClassCar
* @brief         class CLiaoNingFifthClassCar deconstructor that release class resource and
*                call super class deconstrutor.
* @author        luxijia
* @date          2018-07-28
*/
CLiaoNingFifthClassCar::~CLiaoNingFifthClassCar()
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
double CLiaoNingFifthClassCar::calculate_fee(const QString &highway_id, const double distacne)
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
