#ifndef PAYER_DAO_H
#define PAYER_DAO_H

#include <QSqlDatabase>
#include "payer.h"
#include "pool/connection_pool.h"
#include "neu_head.h"
#include "RedisClient/hiredispp.h"

class CPayerDao
{
public:
    CPayerDao();
    CPayerDao(const QSqlDatabase &db);
    ~CPayerDao();

    int query_payers(std::vector<CPayer> &payers,int &all_results_num,const QString &keyword,int page,int page_size);
    int add_payer(CPayer &payer);
    int update_payer(const CPayer &payer);
    int add_balance(const QString &id_card,const double &value);
    int add_balance_by_payer_id(const QString &payer_id,const double &value);
    int query_balance(const QString &id_card,double &balance);
    int quert_balance_by_payer_id(const QString &payer_id,double &balance);
    int valid_payer(const QString &user_name,const QString &password,bool &result);
    int del_payer(const QString &payer_id);
    int query_payer_by_id(CPayer &payer, const QString &payer_id);
    void open_db();


private:

    int add_payer_to_redis(CPayer payer);
    void get_payer_from_redis(CPayer &payer, QString payer_id);
    QSqlDatabase m_db;
    hiredispp::Redis m_rd;
    bool db_from_out;

};

#endif // PAYER_DAO_H
