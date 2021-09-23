#include "add_car_action.h"
IMPLEMENT_ACTION(add_car, CAddCarAction)

void CAddCarAction::run()
{


    try{
        QJsonObject car_json=req->get_json("car");
        CCar car(car_json);
        int status_code=Car::add_car(car);
        if(status_code==StatusCode::SUCCESS){
            resp->put("car_id",car.car_id());
        }
        resp->set_status_code(status_code);

    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

}


