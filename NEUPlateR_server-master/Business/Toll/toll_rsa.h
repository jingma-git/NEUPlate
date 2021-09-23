#ifndef TOLL_RSA_H
#define TOLL_RSA_H

#include "highway.h"
#include "highway_map.h"
#include "highway_toll.h"
#include "neu_head.h"
#define MAX_DISTANCE 32768

namespace MapRSA
{
    void shortest_path(std::vector<CHighWay> &drive_info, const int total_number, CHighWayMap *highway_map,
                       const QString &start, const QString &end);
}
#endif // TOLL_RSA_H
