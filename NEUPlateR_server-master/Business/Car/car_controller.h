#ifndef CAR_CONTROLLER_H
#define CAR_CONTROLLER_H
#include "car_dao.h"

namespace Car {
int add_car(CCar &car);
int unbundle(const QString &car_id);
int bundle(const QString &payer_id,const QString &car_id);
int del_car(const QString &car_id);
int update_car(const CCar &car);
int query_cars_by_payer(QList<CCar> &cars,const QString &payer_id);
int query_car_by_plate(CCar &car,const QString &plate);
int query_cars(std::vector<CCar> &cars,int &all_page_num,const QString &keyword,int page,int page_size);


}

#endif // CAR_CONTROLLER_H
