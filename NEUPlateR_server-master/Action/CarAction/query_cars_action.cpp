#include "query_cars_action.h"
IMPLEMENT_ACTION(query_cars, CQueryCarsAction)

void CQueryCarsAction::run()
{

    try{
        QString keyword=req->get_string("keyword");
        int page=req->get_int("page");
        int page_size=req->get_int("page_size");
        std::vector<CCar> cars;
        int all_pages;
        int result= Car::query_cars(cars,all_pages,keyword,page,page_size);

        QJsonArray qjs;
        unsigned int i;
        for(i=0;i<cars.size();i++){
            qjs.append(QJsonValue(cars.at(i).toJSON()));
        }
        resp->put("cars",qjs);
        resp->put("all_pages",all_pages);
        resp->set_status_code(result);
        return ;

    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

}


