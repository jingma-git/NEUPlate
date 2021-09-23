#ifndef CCAR_DAO_H
#define CCAR_DAO_H

#include "pool/connection_pool.h"
#include "RedisClient/hiredispp.h"
#include "car.h"
#include "neu_head.h"

class CCarDao
{
public:
    CCarDao();
    ~CCarDao();
    int add_car(CCar &car);
    int bundleAndUnboundle(const QString &payer_id,const QString &car_id);
    int del_car(const QString &car_id);
    int update_car(const CCar &car);
    int query_cars_by_payer(QList<CCar> &cars,const QString &payer_id);
    int query_car_by_plate(CCar &car,const QString &plate);
    int query_cars(std::vector<CCar> &cars,int &all_results_num,const QString &keyword,int page,int page_size);


private:
    QSqlDatabase m_db;
    hiredispp::Redis m_rd;


};

#endif // CCAR_DAO_H
