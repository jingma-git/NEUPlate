#ifndef DRIVE_RECOTRD_CONTROLLER_H
#define DRIVE_RECOTRD_CONTROLLER_H

#include "travel_record_dao.h"

namespace TravelRecordController
{
    int add_travel_record_distance(const QString &drive_reord, const double distance);
    int add_start_toll(const QString &start);
    QString add_end_toll(const QString &end, const QString &plate);
    QString get_start_toll(const QString &drive_record);
    QString get_end_toll(const QString &drive_record);
}

#endif // DRIVE_RECOTRD_CONTROLLER_H
