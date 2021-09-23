#include "car_controller.h"


int Car::unbundle(const QString &car_id)
{
    CCarDao car_dao;
    return car_dao.bundleAndUnboundle(NULL,car_id);
}

int Car::bundle(const QString &payer_id, const QString &car_id)
{
    CCarDao car_dao;
    return car_dao.bundleAndUnboundle(payer_id,car_id);
}

int Car::add_car(CCar &car)
{
    CCarDao car_dao;
    return car_dao.add_car(car);
}



int Car::update_car(const CCar &car)
{
    CCarDao car_dao;
    return  car_dao.update_car(car);
}

int Car::query_cars_by_payer(QList<CCar> &cars, const QString &payer_id)
{
    CCarDao car_dao;
    return car_dao.query_cars_by_payer(cars,payer_id);
}

int Car::query_car_by_plate(CCar &car, const QString &plate)
{
    CCarDao car_dao;
    return car_dao.query_car_by_plate(car,plate);
}

int Car::del_car(const QString &car_id)
{
    CCarDao car_dao;
    return car_dao.del_car(car_id);
}

int Car::query_cars(std::vector<CCar> &cars, int &all_page_num, const QString &keyword, int page, int page_size)
{
    CCarDao car_dao;
    int all_results_num;
    int result=car_dao.query_cars(cars,all_results_num,keyword,page,page_size);
    all_page_num=ceil((double)all_results_num/page_size);
    return result;

}
