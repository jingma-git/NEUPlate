#ifndef PLATE_RECOG_H
#define PLATE_RECOG_H

#include <QString>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

namespace PlateRecog{
QString detect_plate_no(char* photo_path);
QString recog_plate_no(cv::Mat &mat);
}
#endif // PLATE_RECOG_H
