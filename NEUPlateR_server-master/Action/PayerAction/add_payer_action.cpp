#include "add_payer_action.h"

IMPLEMENT_ACTION(add_payer,CAddPayerAction)

void CAddPayerAction::run()
{

    QJsonObject payer_json;
    try{
        payer_json=req->get_json("payer");
        CPayer payer(payer_json);
        int status_code=Payer::add_payer(payer);
        resp->set_status_code(status_code);
        if(status_code==StatusCode::SUCCESS){
            resp->put("payer_id",payer.payer_id());
            resp->put("result",true);
        }else{
            resp->put("result",false);
        }
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }


}
