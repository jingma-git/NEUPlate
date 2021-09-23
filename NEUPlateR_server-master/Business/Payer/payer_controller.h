#ifndef PAYER_CONTROLLER_H
#define PAYER_CONTROLLER_H
#include "payer.h"
#include "payer_dao.h"

/**
* @brief
* @author        xiahaoyun
* @date          2018-07-26
* @modify_author xiahaoyun
* @modify_date   2018-07-26
*/

namespace Payer {


int add_payer(CPayer &payer);
int update_payer(const CPayer &payer);
int recharge(const QString &payer_id,const double &value);
int query_balance(const QString &payer_id,double &value);
//int fee_deduction(const QString &payer_id,const double &value);
int valid_payer(const QString &user_name,const QString &password,bool &result);
int query_payers(std::vector<CPayer> &payers, int &all_page_num, const QString &keyword, int page, int page_size); //todo
int del_payer(const QString &payer_id);
int query_payer_by_id(CPayer &payer,const QString &payer_id);

}

#endif // PAYER_CONTROLLER_H
