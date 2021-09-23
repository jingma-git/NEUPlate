#include "recog_record_controller.h"

/**
* @functionName  queryTollStationId
* @Description   query toll station id according to recog record id
* @author        majing
* @date          2018-08-03
* @parameter     recog record id
* @return        toll station id, if the toll station id was not found, return empty string ""
*/

QString RecogRecordController::queryTollStationId(QString record_id){
    CRecogRecordDao recordDao;
    return recordDao.queryTollStationId(record_id);
}

/**
* @functionName  queryPhotoPath
* @Description   query photo_path according to recog record id
* @author        majing
* @date          2018-08-03
* @parameter     recog record id
* @return        photo_path, if the photo_path was not found, return empty string ""
*/
QString RecogRecordController::queryPhotoPath(QString record_id){
    CRecogRecordDao recordDao;
    return recordDao.queryPhotoPath(record_id);
}


QString RecogRecordController::queryPlateNo(QString record_id){
    CRecogRecordDao recordDao;
    return recordDao.queryPlateNo(record_id);
}
/**
* @functionName  addRecogRecord
* @Description   brief
* @author        majing
* @date          2018-08-03
* @parameter     void
* @return        status code
*/

int RecogRecordController::addRecogRecord(CRecogRecord &record){
    CRecogRecordDao recogDao;
    return recogDao.addRecogRecord(record);
}

int queryEntryRecord(const QString &plate_no, CRecogRecord &record){
    CRecogRecordDao recogDao;
    return recogDao.queryEntryRecord(plate_no, record);
}

int RecogRecordController::queryLastRecogRecord(const QString &plate_no, CRecogRecord &record){
    CRecogRecordDao recogDao;
    return recogDao.queryLastRecogRecord(plate_no, record);
}
