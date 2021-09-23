#include "payer_controller.h"



int Payer::update_payer(const CPayer &payer)
{
    CPayerDao payer_dao;
    return payer_dao.update_payer(payer);
}

int Payer::add_payer(CPayer &payer)
{
    CPayerDao payer_dao;
    return payer_dao.add_payer(payer);
}


int Payer::recharge(const QString &id_card, const double &value)
{
    QSqlDatabase db=ConnectionPool::open_connection();
    CPayerDao payer_dao(db);
    if(value<0){
        return StatusCode::LESS_THEN_ZERO;
    }
    db.transaction();
    int result=payer_dao.add_balance(id_card,value);
    if(StatusCode::SUCCESS==result){
        db.commit();
        return StatusCode::SUCCESS;
    }else{
        db.rollback();
        return result;
    }
}

int Payer::query_balance(const QString &id_card, double &value)
{
    CPayerDao payer_dao;
    return payer_dao.query_balance(id_card,value);
}

int Payer::valid_payer(const QString &user_name, const QString &password,bool &result)
{
    CPayerDao payer_dao;
    return payer_dao.valid_payer(user_name,password,result);
}

int Payer::del_payer(const QString &payer_id)
{
    CPayerDao payer_dao;
    return payer_dao.del_payer(payer_id);
}

int Payer::query_payer_by_id(CPayer &payer, const QString &payer_id)
{
    CPayerDao payer_dao;
    return payer_dao.query_payer_by_id(payer,payer_id);
}

int Payer::query_payers(std::vector<CPayer> &payers, int &all_page_num, const QString &keyword, int page, int page_size)
{
    CPayerDao payer_dao;
    int all_results_num;
    int result=payer_dao.query_payers(payers,all_results_num,keyword,page,page_size);
    all_page_num=ceil((double)all_results_num/page_size);
    return result;
}
