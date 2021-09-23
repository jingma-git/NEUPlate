#ifndef CAR_H
#define CAR_H
#include <QString>
#include <QJsonObject>

class CCar
{
public:
    CCar(const QString &m_plate,const QString &m_car_id,const  QString &m_color,
         const QString &m_model,const QString &m_payer_id,const int &m_type);
    CCar();
    CCar(const QJsonObject &car_json);
    QJsonObject toJSON() const;

    QString plate() const;
    void setPlate(const QString &plate);

    QString car_id() const;
    void setCar_id(const QString &car_id);

    QString color() const;
    void setColor(const QString &color);

    QString model() const;
    void setModel(const QString &model);

    QString payer_id() const;
    void setPayer_id(const QString &payer_id);

    int type() const;
    void setType(int value);

    QString get_type_string();
    QString get_model_string();


    //车情
    static const int MODEL_1; //一类车
    static const int MODEL_2; //二类车
    static const int MODEL_3; //三类车
    static const int MODEL_4; //四类车
    static const int MODEL_5; //五类车

    //车型
    static const int TYPE_NORMAL; //普通车
    static const int TYPE_FREEPAY; //免费车
    static const int TYPE_BLACKLIST; //黑名单车
    static const int TYPE_WANTED; //通缉车

    static QString getCarModelStr(int model_id);
    static QString getCarTypeStr(int type_id);

private:
    QString m_plate;
    QString m_car_id;
    QString m_color;
    QString m_model;
    QString m_payer_id;
    int m_type;
};

#endif // CAR_H
