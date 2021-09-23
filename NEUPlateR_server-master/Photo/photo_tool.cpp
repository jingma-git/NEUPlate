#include "photo_tool.h"
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QUuid>

/**
* @functionName  delete_photo
* @Description   delete photo
* @author        chenhanlin
* @date          2018-07-05
* @parameter     QString photo_path
* @return        void
*/
void PhotoTool::delete_photo(const QString &photo_path)
{
    QFile file(photo_path);
    file.remove();
}

/**
* @functionName  save_photo
* @Description   save the photo on disk, name is generate by random and it will return
                 if failuer, will return empty string
* @author        chenhanlin
* @date          2018-07-01
* @parameter     QByteArray photo_data
* @return        std::string
*/
QString PhotoTool::save_photo(const QByteArray &photo_data)
{
    QUuid id = QUuid::createUuid();
    QString filename(id.toString().remove("{").remove("}").remove("-"));
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
//    QByteArray photo_data(QByteArray::fromBase64(photo_data.toUtf8()));
    file.write(photo_data);
    file.close();
    return filename;
}

/**
* @functionName  XXX
* @Description   brief
* @author        majing
* @date          2018-08-07
* @parameter     void
* @return        void
*/

void PhotoTool::save_photo(QString &photo_path, QString &photoData, int mat_rows, int mat_cols, int mat_type){
    using namespace cv;

    QByteArray photo_byte =  QByteArray::fromBase64(photoData.toUtf8());;
    cv::Mat mat(mat_rows, mat_cols, mat_type, (uchar*)photo_byte.data());
    cv::imwrite(photo_path.toStdString(), mat);

}

/**
* @functionName  load_photo
* @Description   read photo data from disk and encode to base64
* @author        chenhanlin
* @date          2018-07-10
* @parameter     void
* @return        void
*/
QByteArray PhotoTool::load_photo(const QString &photo_path)
{
    QFile file(photo_path);
    file.open(QIODevice::ReadOnly);
    QByteArray data(file.readAll());
    return data;
//    return QString(data.toBase64());
}

QString PhotoTool::to_base64(const QByteArray &photo_data)
{
    return QString(photo_data.toBase64());
}

QByteArray PhotoTool::from_base64(const QString &base64_data)
{
    return QByteArray::fromBase64(base64_data.toUtf8());
}

QString PhotoTool::mat_to_photostr(const cv::Mat &mat){
    //qDebug()<<"PhotoTool::mat_to_photostr";
    QByteArray data((char*)mat.data, mat.cols*mat.rows*3);//
    return QString (data.toBase64());;
}

