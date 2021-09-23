#include "query_payer_by_id_action.h"

IMPLEMENT_ACTION(query_payer_by_id,CQueryPayerByIdAction)

void CQueryPayerByIdAction::run()
{
    QString payer_id;
    try{
    payer_id=req->get_string("payer_id");
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

    CPayer payer;
    int status_code=Payer::query_payer_by_id(payer,payer_id);
    resp->set_status_code(status_code);
    resp->put("payer",payer.toJSON());

}


