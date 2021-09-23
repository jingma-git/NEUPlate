#include "car.h"


CCar::CCar(const QString &m_plate, const QString &m_car_id, const QString &m_color,
           const QString &m_model, const QString &m_payer_id, const int &m_type):
    m_plate(m_plate),m_car_id(m_car_id),m_color(m_color),
    m_model(m_model),m_payer_id(m_payer_id),m_type(m_type)
{

}

CCar::CCar()
{

}

CCar::CCar(const QJsonObject &car_json)
{
    m_plate=car_json.value("plate").toString();
    m_car_id=car_json.value("car_id").toString();
    m_color=car_json.value("color").toString();
    m_model=car_json.value("model").toString();
    m_payer_id=car_json.value("payer_id").toString();
    m_type=car_json.value("type").toInt();
}

QJsonObject CCar::toJSON() const
{
    QJsonObject car_json;
    car_json.insert("plate",m_plate);
    car_json.insert("car_id",m_car_id);
    car_json.insert("color",m_color);
    car_json.insert("model",m_model);
    car_json.insert("payer_id",m_payer_id);
    car_json.insert("type",m_type);
    return car_json;
}

QString CCar::plate() const
{
    return m_plate;
}

void CCar::setPlate(const QString &plate)
{
    m_plate = plate;
}

QString CCar::car_id() const
{
    return m_car_id;
}

void CCar::setCar_id(const QString &car_id)
{
    m_car_id = car_id;
}

QString CCar::color() const
{
    return m_color;
}

void CCar::setColor(const QString &color)
{
    m_color = color;
}

QString CCar::model() const
{
    return m_model;
}

void CCar::setModel(const QString &model)
{
    m_model = model;
}

QString CCar::payer_id() const
{
    return m_payer_id;
}

void CCar::setPayer_id(const QString &payer_id)
{
    m_payer_id = payer_id;
}

int CCar::type() const
{
    return m_type;
}

void CCar::setType(int value)
{
    m_type = value;
}

//车型
const int CCar::MODEL_1=1; //一类车
const int CCar::MODEL_2=2; //二类车
const int CCar::MODEL_3=3; //三类车
const int CCar::MODEL_4=4; //四类车
const int CCar::MODEL_5=5; //五类车

//车情
const int CCar::TYPE_NORMAL=1; //普通车
const int CCar::TYPE_FREEPAY=2; //免费车
const int CCar::TYPE_BLACKLIST=3; //黑名单车
const int CCar::TYPE_WANTED=4; //通缉车

QString CCar::getCarModelStr(int model_id){
    switch(model_id){
    case MODEL_1:
    return "一类车";
    case MODEL_2:
    return "二类车";
    case MODEL_3:
    return "三类车";
    case MODEL_4:
    return "四类车";
    case MODEL_5:
    return "五类车";
    default:
        return "其他";
    }
}
QString CCar::getCarTypeStr(int type_id){
    switch(type_id){
    case TYPE_NORMAL:
    return "普通车";
    case TYPE_FREEPAY:
    return "免费车";
    case TYPE_BLACKLIST:
    return "黑名单车";
    case TYPE_WANTED:
    return "通缉车";
    default:
        return "其他车";
    }
}
