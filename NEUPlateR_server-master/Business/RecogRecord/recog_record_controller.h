#ifndef RECOG_RECORD_CONTROLLER_H
#define RECOG_RECORD_CONTROLLER_H

#include <QString>
#include "recog_record.h"
#include "recog_record_dao.h"

namespace RecogRecordController{
    QString queryTollStationId(QString record_id);
    QString queryPhotoPath(QString record_id);
    QString queryPlateNo(QString record_id);
    int addRecogRecord(CRecogRecord& record);
    int queryEntryRecord(const QString &plate_no, CRecogRecord &record);
    int queryLastRecogRecord(const QString &plate_no, CRecogRecord &record);
}
#endif // RECOG_RECORD_CONTROLLER_H
