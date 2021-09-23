#include "exit_station_action.h"
IMPLEMENT_ACTION(exit_station,CExitStationAction)

void CExitStationAction::run(){
   // Log::debug("CExitStationAction::run()");
    QString station_id;
    int type; //上高速0或下高速1
    int lane_no;
    bool is_etc;
    QString plate_no;
    QJsonObject photoObj;
    QString photo_data;
    int mat_cols;
    int mat_rows;
    int mat_type;
    try{
        //解析客户端发来的数据
        station_id = req->get_string("station_id");
        type = req->get_int("type"); //上高速0或下高速1
        lane_no = req->get_int("lane_no");
        is_etc = req->get_bool("is_etc");
        plate_no = req->get_string("plate_no");
        photoObj = req->get_json("photo");

        photo_data = photoObj.value("photo_data").toString();
        mat_cols = photoObj.value("mat_cols").toInt();
        mat_rows = photoObj.value("mat_rows").toInt();
        mat_type = photoObj.value("mat_type").toInt();
    }catch(NullException e){
        Log::debug("NullException e");
        resp->put("plate_no", plate_no);
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

    //系统中找不到车牌号，有可能识别有问题
    CCar car;
    int status = Car::query_car_by_plate(car, plate_no);
    if(status!=StatusCode::SUCCESS){
        resp->put("plate_no", plate_no);
        resp->set_status_code(StatusCode::RECOG_ERROR);
        return;
    }

    //找不到车辆入站的识别记录，可能出站时识别有问题
    CRecogRecord lastRecogRecord;

    int status_code = RecogRecordController::queryLastRecogRecord(plate_no, lastRecogRecord);

    if(status_code==StatusCode::EMPTY_QUERY || lastRecogRecord.type()==CRecogRecord::EXIT_STATION){
        resp->put("plate_no", plate_no);
        resp->set_status_code(StatusCode::RECOG_ERROR);
        return;
    }

   //产生一条出站识别记录
   // resp->put("car",car.toJSON());
    QDateTime dateTime = QDateTime::currentDateTime();
    QString photo_path = dateTime.toString("yyyyMMddhhmmss")+station_id+QString::number(lane_no);
    photo_path += ".jpg";
    QString dateTimeStr = dateTime.toString("yyyy-MM-dd hh:mm:ss");

    //将图片存起来
    PhotoTool::save_photo(photo_path, photo_data, mat_rows, mat_cols, mat_type);

    //添加一条出站识别记录
    CRecogRecord record;
    record.setPlateNo(plate_no);
    record.setDateTime(dateTimeStr);
    record.setTollStation(station_id);
    record.setPhotoPath(photo_path);
    record.setLaneNo(lane_no);
    record.setType(type);
    record.setIsEtc(is_etc);

    RecogRecordController::addRecogRecord(record);

    //扣费
    QString travel_id =TravelRecordController::add_end_toll(record.record_id(),plate_no);
    if(travel_id.isEmpty()){
        resp->set_status_code(StatusCode::DEDUCT_FEE_FAIL);
        return;
    }


    //账户余额不足，不允许自动扣费
    double fee = DeductionController::manual_deduction(travel_id, car.model(), car.type());

    if(fee<0){
        resp->set_status_code(StatusCode::DEDUCT_FEE_FAIL);
        return;
    }else{
        int dedStatus = DeductionController::add_deduction_record_by_account(car.payer_id(),travel_id,station_id,pay_method::PAY_BY_ACCOUNT,fee);
        if(dedStatus==StatusCode::FEE_NO_ENOUGH){
            resp->put("travel_id", travel_id);
        }
        resp->put("fee", fee);
        resp->set_status_code(dedStatus);
        return;
    }
}
