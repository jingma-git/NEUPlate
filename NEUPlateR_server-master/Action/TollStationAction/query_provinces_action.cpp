#include "query_provinces_action.h"

IMPLEMENT_ACTION(query_provinces,CQueryProvincesAction)
void CQueryProvincesAction::run()
{
    std::vector<QString> provinces;
    int status = TollStationController::query_provinces(provinces);

    if(status==StatusCode::SUCCESS){
        QJsonArray pro_array;
        for(QString province:provinces){
            pro_array.append(province);
        }
        resp->put("provinces", pro_array);
    }
    resp->set_status_code(status);
}
