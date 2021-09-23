#include "enter_station_action.h"
#include <iostream>

IMPLEMENT_ACTION(enter_station,CEnterStationAction)

void CEnterStationAction::run()
{
   // Log::debug("CEnterStationAction::run()");
    try{
        //对服务端的传来的json数据进行解码
        QString station_id = req->get_string("station_id");
        int type = req->get_int("type");
        int lane_no = req->get_int("lane_no");
        bool is_etc = req->get_bool("is_etc");
        QString plate_no = req->get_string("plate_no");
        QJsonObject photoObj = req->get_json("photo");


        //已经有同样车牌号的车上了高速,说明是套牌车,不予放行
        CRecogRecord lastRecogRecord;
        int status_code =
                RecogRecordController::queryLastRecogRecord(plate_no, lastRecogRecord);
        if(status_code!=StatusCode::EMPTY_QUERY){

            if(lastRecogRecord.type()==CRecogRecord::ENTER_STATATION){
                resp->set_status_code(StatusCode::DUPLICATE_PLATE);
                return;
            }
        }

        //如果根据车牌号查询不出对应的车辆信息，则不予放行
        CCar car;
        int status = Car::query_car_by_plate(car, plate_no);


        if(status!=StatusCode::SUCCESS){
            resp->set_status_code(StatusCode::EMPTY_QUERY);
            return;
        }

        //如果车辆被通缉，不予放行
        if(car.type()==CCar::TYPE_WANTED){
            resp->set_status_code(StatusCode::IN_WANTED);
            return;
        }

        //如果成功地查询到车辆信息，则放行，并且产生一条进站识别记录
        //根据当前日期和传来的站点信息产生record_id
        QDateTime dateTime = QDateTime::currentDateTime();
        QString photo_path = dateTime.toString("yyyyMMddhhmmss")+station_id+QString::number(lane_no);
        photo_path += ".jpg";
        QString dateTimeStr = dateTime.toString("yyyy-MM-dd hh:mm:ss");

        //将图片存起来
        QString photo_data = photoObj.value("photo_data").toString();
        int mat_cols = photoObj.value("mat_cols").toInt();
        int mat_rows = photoObj.value("mat_rows").toInt();
        int mat_type = photoObj.value("mat_type").toInt();

        PhotoTool::save_photo(photo_path, photo_data, mat_rows, mat_cols, mat_type);

        //新增一条识别记录
        CRecogRecord record;
        record.setPlateNo(plate_no);
        record.setDateTime(dateTimeStr);
        record.setTollStation(station_id);
        record.setPhotoPath(photo_path);
        record.setLaneNo(lane_no);
        record.setType(type);
        record.setIsEtc(is_etc);

        int code = RecogRecordController::addRecogRecord(record);
        if(StatusCode::SUCCESS != code){
            resp->set_status_code(StatusCode::SYSTEM_ERROR);
            return;
        }
        code = TravelRecordController::add_start_toll(record.record_id());
        if(1 != code){
            resp->set_status_code(StatusCode::SYSTEM_ERROR);
            return;
        }
        resp->set_status_code(StatusCode::SUCCESS);

    }catch(NullException e){
        Log::debug("NullException e");
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

}
