#include "valid_payer_action.h"
IMPLEMENT_ACTION(valid_payer, CValidPayerAction)

void CValidPayerAction::run()
{
    QString username;
    QString password;
    try{
        username=req->get_string("username");
        password=req->get_string("password");

    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

    bool result;
    int status_code=Payer::valid_payer(username,password,result);
    resp->set_status_code(status_code);
    resp->put("result",result);
}


