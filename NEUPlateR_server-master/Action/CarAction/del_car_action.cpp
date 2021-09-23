#include "del_car_action.h"
IMPLEMENT_ACTION(delete_car, CDeleteCarAction)

void CDeleteCarAction::run()
{

    try{
        QString car_id=req->get_string("car_id");
        int status_code=Car::del_car(car_id);
        resp->set_status_code(status_code);


    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

}


