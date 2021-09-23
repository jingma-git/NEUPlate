#ifndef TOLL_CONTROLLER_H
#define TOLL_CONTROLLER_H

/**
* @namespace     TollController
* @brief         This namespace deal with toll information request.
* @author        luxijia
* @date          2018-7-25
*/

#include "toll_dao.h"
#include "highway.h"
#include "toll_rsa.h"
#include "highway_map.h"

namespace TollController
{
    std::vector<CHighWay> get_path(const QString &start_place, const QString &end_place);
    QString get_location(const QString &id);
}

#endif // TOLL_CONTROLLER_H
