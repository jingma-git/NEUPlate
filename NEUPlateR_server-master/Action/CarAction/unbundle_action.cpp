#include "unbundle_action.h"
IMPLEMENT_ACTION(unbundle_car, CUnbunleCarAction)

void CUnbunleCarAction::run()
{

    try{

        QString car_id=req->get_string("car_id");
        int status_code=Car::unbundle(car_id);
        resp->set_status_code(status_code);
        resp->put("car_id",car_id);


    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

}


