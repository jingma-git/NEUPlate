#ifndef PHOTOTOOL_H
#define PHOTOTOOL_H

#include <QString>
#include <QByteArray>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace PhotoTool {
    QString save_photo(const QByteArray &photo_data);
    void save_photo(QString &photo_path, QString &photoData, int mat_rows, int mat_cols, int mat_type);
    void delete_photo(const QString &photo_path);
    QByteArray load_photo(const QString &photo_path);
    QString to_base64(const QByteArray &photo_data);
    QByteArray from_base64(const QString &base64_data);
    QString mat_to_photostr(const cv::Mat &mat);
}

#endif // PHOTOTOOL_H
