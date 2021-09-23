#include "toll_rsa.h"
#include "QDebug"

/**
* @name          shortest_path
* @brief         calulate the shortest path beteween two two station.
* @author        luxijia
* @date          2018-07-27
* @param         drive_ino the path inforamation from start place to end place.
* @param         total_number the all toll stations number in special province.
* @param         highway_map the toll stations map information.
* @param         start the name/id of start toll station
* @param         end the name/id of end toll station
*/
void MapRSA::shortest_path(std::vector<CHighWay> &drive_info, const int total_number, CHighWayMap *highway_map,
                           const QString &start, const QString &end)
{
    int start_index = highway_map->find_toll_index(start);
    int end_index = highway_map->find_toll_index(end);

    if (end_index == -1)
        return;

    int i = 0;
    int count = 0;
    int recall = 0;
    double min_distance;
    int min_index;
    int from_toll[total_number];
    int is_calcul[total_number];
    double distance[total_number];
    CLinkToll *temp = highway_map->toll_array()[start_index].head()->next();

    //init start linl toll distance
    for (i = 0; i < total_number; i++)
    {
        from_toll[i] = -1;
        distance[i] = MAX_DISTANCE;
        is_calcul[i] = 0;
    }

    is_calcul[start_index] = 1;
    distance[start_index] = 0;

    while (NULL != temp)
    {
        distance[temp->index()] = temp->distance();
        temp = temp->next();
    }

    //dijstra algorithm
    while (count < total_number)
    {
        //find min distnce in link toll distance.
        min_distance = MAX_DISTANCE;
        min_index = -1;

        for (i = 0; i < total_number; i++)
        {
            if (0 == is_calcul[i] && i != start_index && distance[i] <= min_distance)
            {
                min_distance = distance[i];
                min_index = i;
            }
        }

        if (-1 == min_index)
        {
            count++;
            continue;
        }

        is_calcul[min_index] = 1;

        temp = highway_map->toll_array()[min_index].head()->next();

        while (NULL != temp)
        {
            if ((0 == is_calcul[temp->index()]) && (min_distance + temp->distance() < distance[temp->index()]))
            {
                from_toll[temp->index()] = min_index;
                distance[temp->index()] = min_distance + temp->distance();
            }

            temp = temp->next();
        }

        count++;

        if (end == highway_map->toll_array()[min_index].toll_name())
            break;
    }

    for (i = end_index, recall = from_toll[i]; recall != -1;)
    {
        CHighWay highway;
        highway.setStart(highway_map->toll_array()[recall].toll_name());
        highway.setEnd(highway_map->toll_array()[i].toll_name());
        highway.setDistance(distance[i] - distance[recall]);
        drive_info.push_back(highway);
        i = recall;
        recall = from_toll[i];
    }

    CHighWay highway;
    highway.setStart(highway_map->toll_array()[start_index].toll_name());
    highway.setEnd(highway_map->toll_array()[i].toll_name());
    highway.setDistance(distance[i] - distance[start_index]);
    drive_info.push_back(highway);
}
