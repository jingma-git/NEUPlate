#include "query_stations_action.h"

IMPLEMENT_ACTION(query_stations,CQueryStationsAction)

void CQueryStationsAction::run(){
    QString province = req->get_string("province");
    std::vector<CTollStation> stations;
    int status = TollStationController::query_stations(province, stations);
    if(status==StatusCode::SUCCESS){
        QJsonArray station_array;
        for(const CTollStation& station:stations){
            station_array.append(station.toJSON());
        }
        resp->put("stations", station_array);
    }
    resp->set_status_code(status);
}
