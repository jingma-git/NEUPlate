#include "test_recog_record_controller.h"

void TestRecogRecord::test_query_photo_path(){
    QString photo_path = RecogRecordController::queryPhotoPath("1");
    Log::debug(QString("photo_path: %1").arg(photo_path));
}

void TestRecogRecord::test_query_station_id(){
    QString station_id = RecogRecordController::queryTollStationId("1");
    Log::debug(QString("toll station id: %1").arg(station_id));
}
