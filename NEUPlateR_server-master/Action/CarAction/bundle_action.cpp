#include "bundle_action.h"
IMPLEMENT_ACTION(bundle_car, CBundleCarAction)

void CBundleCarAction::run()
{


    try{
        QString payer_id=req->get_string("payer_id");
        QString car_id=req->get_string("car_id");
        int status_code= Car::bundle(payer_id,car_id);
        resp->put("payer_id",payer_id);
        resp->put("car_id",car_id);
        resp->set_status_code(status_code);

    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

}


