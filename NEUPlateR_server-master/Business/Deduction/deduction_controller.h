#ifndef DEDUCTION_CONTROLLER_H
#define DEDUCTION_CONTROLLER_H

#include "Business/Toll/toll_controller.h"
#include "Business/TravelRecord/travel_record_controller.h"
#include "Business/Toll/highway.h"
#include "Business/RecogRecord/recog_record_controller.h"
#include "Business/Car/car_controller.h"
#include "deduction_dao.h"
#include "deduction_factory.h"
#include "ProvinceDeduction/deduction_head.h"
#include "CarTypeDeduction/car_type_deduction_head.h"
#include "deduction_record.h"

enum dedution_type
{
    DEDUCTION_BY_DISTANCE = 1,
    DEDUCTION_BY_TIMES
};

enum pay_method
{
    PAY_BY_MONEY = 1,
    PAY_BY_ACCOUNT
};

enum car_situation
{
    NORMAL_CAR = 1,
    FREE_CAR,
    BACKLIST_CAR,
    WANTED_CAR
};

namespace DeductionController
{
    int get_deduction(std::vector<CDedutionRecord> &records, int &all_row_number, const QString &keyword,
                      const QString &start_time, const QString &end_time, const int state, const int page_number,
                      const int page_item);
    int delete_deduction(const QString &deduction_id);
    int is_valid_reversal_deduction(QString &drive_id, const QString &deduction_id);
    int reversal_deduction(const QString &deduction_id, const QString &new_deduction_id);
    int reversal_deduction_by_plate(const QString &deduction_id, const QString &plate);
    double auto_deduction(const QString &payer_id, const QString &drive_record, const QString &car_type, const int car_situation);
    double manual_deduction(const QString &drive_record, const QString &car_type, const int &car_situation);
    int correct_error_palte(const QString &recogonition_id, const QString &plate);
    int add_deduction_record(const QString &drive_record, const QString &end_toll, const double fee);
    int add_deduction_record_by_account(const QString &payer_id, const QString &drive_record, const QString &end_toll, const int pay_method, const double fee);
}

#endif // DEDUCTION_CONTROLLER_H
