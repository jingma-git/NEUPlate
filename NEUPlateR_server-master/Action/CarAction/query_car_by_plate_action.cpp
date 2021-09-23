#include "query_car_by_plate_action.h"
IMPLEMENT_ACTION(query_car_by_plate, CQueryCarByPlateAction)

void CQueryCarByPlateAction::run()
{

    try{
        CCar car;
        QString plate=req->get_string("plate");
        int status_cdoe= Car::query_car_by_plate(car,plate);
        resp->set_status_code(status_cdoe);
        if(status_cdoe==StatusCode::SUCCESS){
            resp->put("car",car.toJSON());
        }

    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

}


