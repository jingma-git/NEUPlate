#ifndef RECOG_RECORD_DAO_H
#define RECOG_RECORD_DAO_H

#include <QSqlDatabase>
#include "recog_record.h"
#include "status_code.h"
#include "pool/connection_pool.h"
#include "recog_record.h"
#include "log/log_handler.h"

class CRecogRecordDao
{
public:
    CRecogRecordDao();
    ~CRecogRecordDao();
    int addRecogRecord(CRecogRecord& record);
    int queryEntryRecord(const QString &plate_no, CRecogRecord &record);
    int queryLastRecogRecord(const QString &plate_no, CRecogRecord &record);
    QString queryTollStationId(QString record_id);
    QString queryPhotoPath(QString record_id);
    QString queryPlateNo(QString record_id);

private:
    QSqlDatabase db;
};

#endif // DRIVE_RECORD_DAO_H
