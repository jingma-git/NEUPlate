#include "payer_recharge_action.h"

IMPLEMENT_ACTION(recharge_payer, CRechargePayerAction)

void CRechargePayerAction::run()
{
    QString id_card;
    double value=req->get_double("value");
    try{
        id_card=req->get_string("id_card");
        value=req->get_double("value");
        int status_code=Payer::recharge(id_card,value);
        resp->set_status_code(status_code);
        if(status_code==StatusCode::SUCCESS){
            resp->put("id_card",id_card);
        }
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

}


