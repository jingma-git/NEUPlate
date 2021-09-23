/**
* @projectName   plate number recognize 车牌识别
* @brief         brief
* @author        majing
* @date          2018-07-26
* @modify_author XXXX
* @modify_date   2018-07-26
*/

#include "plate_recog.h"
#include "log/log_handler.h"

/**
* @functionName  detect_plate_no
* @Description   brief
* @author        majing
* @date          2018-07-26
* @parameter     the path of photo which is being detected
* @return        empty string if nothing is detected
*/

QString PlateRecog::detect_plate_no(char* photo_path){
    /*
    CPlateRecognize pr;
    pr.setDetectType(PR_DETECT_CMSER|PR_DETECT_COLOR);
    vector<CPlate> plateVec;

    cv::Mat img;

    img = cv::imread(photo_path);
    if(img.empty()){
        Log::debug("empty img");
        return "";
    }
    pr.setResultShow(false);
    int result = pr.plateRecognize(img, plateVec);
    pr.getDetectShow();
    CPlate plate=plateVec.at(0);
    std::string license = plate.getPlateStr();
    QString lice1 = QString::fromLocal8Bit(license.c_str());
    return lice1;
    */
    return "";
}

QString PlateRecog::recog_plate_no(cv::Mat &mat){
    /*
    qDebug()<<"PlateRecog::recog_plate_no";
    CPlateRecognize pr;
    pr.setDetectType(PR_DETECT_CMSER|PR_DETECT_COLOR);
    vector<CPlate> plateVec;
    pr.setResultShow(false);
    int result = pr.plateRecognize(mat, plateVec);
    qDebug()<<"int result "<<QString::number(result);
    pr.getDetectShow();
    qDebug()<<"before CPlate plate=plateVec.at(0);";
    CPlate plate=plateVec.at(0);
    std::string license = plate.getPlateStr();
    QString lice1 = QString::fromLocal8Bit(license.c_str());
    qDebug()<<".........."<<lice1;
    return lice1;
    */
    return "";
}
