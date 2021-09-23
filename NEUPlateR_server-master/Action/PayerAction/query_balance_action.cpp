#include "query_balance_action.h"

IMPLEMENT_ACTION(query_balance, CQueryBalanceAction)

void CQueryBalanceAction::run()
{
    QString id_card;
    try{
    id_card=req->get_string("id_card");

    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }
    double balance;

    int status_code=Payer::query_balance(id_card,balance);
    resp->set_status_code(status_code);
    resp->put("balance",balance);
}


