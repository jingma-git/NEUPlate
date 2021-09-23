#include "enter_exit_station_controller.h"

/**
* @functionName  识别车牌号
* @Description   brief
* @author        majing
* @date          2018-07-26
* @parameter     void
* @return        void
*/

void EnterExitStationController::recog_plate_no(){
    //从req(station_id,lane_no,photo)得到图片
    //resp返回车牌号
}

/**
* @functionName  决定是否放行
* @Description   brief
* @author        majing
* @date          2018-07-26
* @parameter     void
* @return        void
*/

void EnterExitStationController::car_enter(){
    //得到station_id,lane_no,完全正确的车牌号,上传的车牌图片photo_path,is_etc

    //根据plate_no调用CarInfo得到车型，车情，车辆的余额是否充足（是否在黑名单中）

    //根据lane_no得到是否是人工收费还是ETC通道
    //如果是人工收费通道-放行
    //增加一条识别记录
    // plate = plate_no,time,toll_station,type=ENTER_STATATION,photo_path,lane_no,is_etc=false;
    //如果是ETC通道-不放行
}

void EnterExitStationController::car_exit(){
    //得到station_id,lane_no,完全正确的车牌号,上传的车牌图片photo_path,is_etc

    //根据plate_no调用CarInfo得到车型，车情

    //根据车牌号得到该车最近一次的进站记录

    //如果出站时是人工收费通道-调用收费模块（传入start,end)得到扣费金额
    //增加一条识别记录
    // plate = plate_no,time,toll_station,type=ENTER_STATATION,photo_path,lane_no,is_etc=false;
    //如果是ETC通道-调用收费模块（传入start,end)自动扣费
}

