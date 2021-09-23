#include "update_payer_action.h"
IMPLEMENT_ACTION(update_payer, CUpdatePayerAction)

void CUpdatePayerAction::run()
{
    QJsonObject payer_json;
    try{
        payer_json=req->get_json("payer");

        CPayer payer=CPayer(payer_json);
        int status_code=Payer::update_payer(payer);
        resp->set_status_code(status_code);
        if(status_code==StatusCode::SUCCESS){
            resp->put("payer_id",payer.payer_id());
        }
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

}


