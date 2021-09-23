#include "update_car_action.h"
IMPLEMENT_ACTION(update_car, CUpdateCarAction)

void CUpdateCarAction::run()
{

    try{
        QJsonObject car_json=req->get_json("car");
        CCar car(car_json);

        int status_code=Car::update_car(car);
        resp->put("car_id",car.car_id());
        resp->set_status_code(status_code);
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

}


