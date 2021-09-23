#include "toll_controller.h"

/**
* @name          get_location
* @brief         get toll station location.
* @author        luxijia
* @date          2018-07-25
* @param         id the toll station id
* @param         location the place location, such as province name or city name
* @return        QString if get distance success, then
*/
QString TollController::get_location(const QString &id)
{
    CTollDAO toll_dao;
    QString location;
    int code;

    code = toll_dao.query_toll_location(location, id);

    if (StatusCode::SUCCESS == code)
        return location;
    else
        return "";
}

/**
* @name          get_distance
* @brief         calculate car run distacne from start palce to end place.
* @author        luxijia
* @date          2018-7-25
* @param         distance the tow place distance.
* @param         start_id start place id
* @param         end_id end place id
* @return        std::vector if has a distance then return a non-empty vector.
*                if no distance then return a empty vector.
*/
std::vector<CHighWay> TollController::get_path(const QString &start_place, const QString &end_place)
{
    int code;
    int total_number;
    int size;
    QString location;
    std::vector<CHighWay> drive_info;
    std::vector<QString> tolls;
    CTollDAO toll_dao;

    code = toll_dao.query_toll_location(location, start_place);

    if (StatusCode::SUCCESS != code)
    {
        Log::debug("query toll location failed.");
        return drive_info;
    }

    code = toll_dao.query_total_toll(tolls, total_number, location);

    if (StatusCode::SUCCESS != code)
    {
        Log::debug("query total toll station failed.");
        return drive_info;
    }

    //get highway location map
    CHighWayMap highway_map(total_number, location);
    highway_map.setToll_array(new CHighWayToll[total_number]);

    //initial map inforamtion
    for (int i = 0; i < total_number; i++)
        highway_map.set_toll_attay(i, tolls[i]);


    //get location pronvince all toll station inforamtion
    code = toll_dao.query_toll_distance(total_number, &highway_map);

    if (StatusCode::SUCCESS != code)
    {
        Log::debug("query toll location failed.");
        return drive_info;
    }

    //calculate shortest path
    if (start_place != end_place)
        MapRSA::shortest_path(drive_info, total_number, &highway_map, start_place, end_place);
    else
    {
        CLinkToll *temp = highway_map.toll_array()[highway_map.find_toll_index(start_place)].head()->next();

        while (temp != NULL)
        {
            if (end_place == highway_map.toll_array()[temp->index()].toll_name())
            {
                CHighWay highway;
                highway.setStart(start_place);
                highway.setEnd(end_place);
                highway.setDistance(temp->distance());
            }

            temp = temp->next();
        }
    }

    size = drive_info.size();

    for (int i = 0; i < size; i++)
    {
        QString id = "";
        toll_dao.query_highway_id(id, drive_info[i].start(), drive_info[i].end());
        drive_info[i].setId(id);
    }

    return drive_info;
}
