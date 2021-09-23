#ifndef DEDUCTION_DAO_H
#define DEDUCTION_DAO_H

#include "RedisClient/hiredispp.h"
#include "neu_head.h"
#include "Business/Payer/payer_dao.h"
#include "pool/connection_pool.h"
#include "deduction_record.h"

enum deduction_state
{
    REVERSAL =0,
    FINISH
};

/**
* @class         CDeductionDAO
* @brief         This class deal with request with database
*                about Deduction information.
* @author        luxijia
* @date          2018-07-27
*/
class CDeductionDAO
{
public:
    CDeductionDAO();
    CDeductionDAO(const QSqlDatabase &db);
    ~CDeductionDAO();
    int insert_deduction_record(const QString &deduction_id, const QString &deduction_time, const QString &drive_record,
                                const QString &plate, const int pay_method, const double fee);
    int insert_deduction_record_by_payer(const QString &payer_id, const QString &deduction_id, const QString &deduction_time,
                                const QString &drive_record, const QString &plate, const int pay_method, const double fee);
    int insert_reversal_deduction_record(const QString &deduction_id, const QString &new_deduction_id);
    int insert_reversal_deduction_record_with_account(const QString &deduction_id, const QString &reversal_deduction_one,
                                                      const QString &reversal_deduction_two, const QString &plate,const QString &payer_id);
    int insert_reversal_deduction_record_no_account(const QString &deduction_id, const QString &reversal_deduction_one,
                                                    const QString &reversal_deduction_two);
    int insert_correct_error_plate(const QString &recognition_id, const QString &plate);
    int query_charge_type(int &charge_type, const QString &car_type, const QString &highway_id);
    int query_charge_price(double &price, const QString &car_type, const QString &highway_id);
    int query_basic_price(double &basic_price, const QString &car_type, const QString &highway_id);
    int query_deduction(std::vector<CDedutionRecord> &records, int &all_row_number, const QString &keyword,
                        const QString &start_time, const QString &end_time,
                        const int state, const int offset, const int query_number);
    int query_reversal_deduction_validity(QString &drive_id, const QString &deduction_id);
    int delete_deduction_record(const QString &deduction_id);
private:
    static QString DEDUCTION_CHARGE_TYPE;
    static QString DEDUCTION_CHARGE_PRICE;
    static QString DEDUCTION_BASIC_PRICE;
    static QString DEDUCTION_STATE_REVERSAL;
    QSqlDatabase m_db;
    hiredispp::Redis m_redis;
};

#endif // DEDUCTION_DAO_H
