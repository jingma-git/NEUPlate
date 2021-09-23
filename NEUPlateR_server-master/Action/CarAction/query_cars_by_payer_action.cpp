#include "query_cars_by_payer_action.h"
IMPLEMENT_ACTION(query_cars_by_payer, CQueryCarsByPayerAction)

void CQueryCarsByPayerAction::run()
{

    try{
        QString payer_id=req->get_string("payer_id");
        QList<CCar> cars;
        QJsonArray cars_array;
        int status_code=Car::query_cars_by_payer(cars,payer_id);
        resp->set_status_code(status_code);
        if(status_code==StatusCode::SUCCESS){
            for(int i=0;i<cars.length();i++){
                cars_array.append(QJsonValue(cars.at(i).toJSON()) );
            }
            resp->put("cars",cars_array);
        }

    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

}


