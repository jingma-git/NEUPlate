/**
* @projectName   进出站记录
* @brief         brief
* @author        majing
* @date          2018-07-26
* @modify_author XXXX
* @modify_date   2018-07-26
*/
#include "recog_record_dao.h"

CRecogRecordDao::CRecogRecordDao()
{
    db = ConnectionPool::open_connection();
}

CRecogRecordDao::~CRecogRecordDao()
{
   ConnectionPool::close_connection(db);
}

int CRecogRecordDao::addRecogRecord(CRecogRecord& record){

    QSqlQuery query(db);

    query.prepare("CALL add_recog_record(:plate_no,:date_time,:toll_station,"
                  ":type,:photo_path,:lane_no,:is_etc, @record_id, @error_code)");

    query.bindValue(":plate_no", record.plate_no());
    query.bindValue(":date_time", record.date_time());
    query.bindValue(":toll_station", record.toll_station());
    query.bindValue(":type", record.type());
    query.bindValue(":photo_path", record.photo_path());
    query.bindValue(":lane_no", record.lane_no());
    query.bindValue(":is_etc", record.is_etc());

    query.exec();

    query.exec("SELECT @record_id;");
    query.next();
    QString record_id = query.value(0).toString();
    record.setRecordId(record_id);

    query.exec("SELECT @error_code;");
    query.next();
    int success = query.value(0).toInt();
    if(1 == success){
        Log::debug(query.lastError().text());
        return StatusCode::SQL_EXEC_ERROR;
    }else{
        return StatusCode::SUCCESS;
    }
}


int CRecogRecordDao::queryEntryRecord(const QString &plate_no, CRecogRecord &record){
    QSqlQuery query(db);

    query.prepare("SELECT * FROM recognition_log \
                  WHERE plate_no = :plate_no AND type=0 \
                  ORDER BY time DESC LIMIT 0,1;");

    query.bindValue(":plate_no", plate_no);

    if(!query.exec()){
        return StatusCode::SQL_EXEC_ERROR;
    }

    if(query.next()){
        record.setRecordId(query.value("id").toString());
        record.setTollStation(query.value("toll_station").toString());
        record.setDateTime(query.value("time").toString());
        record.setLaneNo(query.value("lane_no").toInt());
        record.setPlateNo(query.value("plate_no").toString());
        record.setPhotoPath(query.value("photo_path").toString());
        record.setType(query.value("type").toInt());
        record.setIsEtc(query.value("is_etc").toBool());
    }else{
        return StatusCode::EMPTY_QUERY;
    }
    return StatusCode::SUCCESS;
}

/**
* @functionName  查询最后上高速的记录
* @Description   brief
* @author        majing
* @date          2018-08-03
* @parameter     void
* @return        void
*/

int CRecogRecordDao::queryLastRecogRecord(const QString &plate_no, CRecogRecord &record){
    //qDebug()<<"queryLastRecogRecord";
    QSqlQuery query(db);

    query.prepare("SELECT * FROM recognition_log \
                  WHERE plate_no = :plate_no \
                  ORDER BY time DESC LIMIT 0,1;");

    query.bindValue(":plate_no", plate_no);

    if(!query.exec()){
        return StatusCode::SQL_EXEC_ERROR;
    }

    if(query.next()){

        record.setRecordId(query.value("id").toString());
        record.setTollStation(query.value("toll_station").toString());
        record.setDateTime(query.value("time").toString());
        record.setLaneNo(query.value("lane_no").toInt());
        record.setPlateNo(query.value("plate_no").toString());
        record.setPhotoPath(query.value("photo_path").toString());
        record.setType(query.value("type").toInt());
        record.setIsEtc(query.value("is_etc").toBool());
    }else{
        return StatusCode::EMPTY_QUERY;
    }
    return StatusCode::SUCCESS;
}

QString CRecogRecordDao::queryTollStationId(QString record_id){

    QSqlQuery query(db);
    query.prepare("SELECT toll_station FROM recognition_log WHERE id=?");
    query.addBindValue(record_id);

    if(!query.exec()){
        return "";
    }
    if(!query.first()){
        return "";
    }
    QString toll_id = query.value("toll_station").toString();
    return toll_id;
}

QString CRecogRecordDao::queryPhotoPath(QString record_id){

    QSqlQuery query(db);
    query.prepare("SELECT photo_path FROM recognition_log WHERE id=?");
    query.addBindValue(record_id);

    if(!query.exec()){
        return "";
    }
    if(!query.first()){
        return "";
    }
    QString photo_path = query.value("photo_path").toString();
    return photo_path;
}

QString CRecogRecordDao::queryPlateNo(QString record_id){
    QSqlQuery query(db);
    query.prepare("SELECT plate_no FROM recognition_log WHERE id=?");
    query.addBindValue(record_id);

    if(!query.exec()){
        return "";
    }
    if(!query.first()){
        return "";
    }
    QString plate_no = query.value("plate_no").toString();
    return plate_no;
}
