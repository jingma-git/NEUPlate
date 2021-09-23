#include "del_payer_action.h"
IMPLEMENT_ACTION(delete_payer,CDelPayerAction)

void CDelPayerAction::run()
{
    QString payer_id;
    try{
        payer_id=req->get_string("payer_id");
        int status_code=Payer::del_payer(payer_id);

        resp->set_status_code(status_code);

        if(status_code==StatusCode::SUCCESS){
            resp->put("payer_id",payer_id);
        }
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

}


