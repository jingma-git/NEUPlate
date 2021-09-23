#include "recog_plate_action.h"
#include <sys/time.h>

IMPLEMENT_ACTION(recog_plate,CRecogPlateAction)

void CRecogPlateAction::run(){
  //  double start =(double) clock();
    Log::debug("CRecogPlateAction::run()");
    QString photo=req->get_string("photo");
    int width = req->get_int("width");
    int height = req->get_int("height");
    int type = req->get_int("type");
    QByteArray photo_byte = PhotoTool::from_base64(photo);
    cv::Mat mat(width, height, type, (uchar*)photo_byte.data());
    cv::imwrite("myPhoto.jpg", mat);

    QString plate_no = PlateRecog::detect_plate_no("myPhoto.jpg");
    Log::debug(plate_no);
//    double end = (double) clock();
//    qDebug()<<(end - start)/1000.0<<"s";
    resp->put("plate_no", plate_no);

  //  cv::imshow("photo", mat);
  //  cv::waitKey(0);
}

