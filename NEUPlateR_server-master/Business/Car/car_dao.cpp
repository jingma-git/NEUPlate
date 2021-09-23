#include "car_dao.h"


#include <iostream>

CCarDao::CCarDao():m_db(ConnectionPool::open_connection()),
    m_rd("localhost")
{
}

CCarDao::~CCarDao()
{
    ConnectionPool::close_connection(m_db);
}

int CCarDao::add_car(CCar &car)
{

    QSqlQuery query(m_db);
    m_db.transaction();
    QString car_id;
    bool success = query.exec("SELECT A.id FROM id_contain AS A JOIN (SELECT CEIL(MAX(CAST(id AS SIGNED))*RAND())\
                             AS id FROM id_contain) AS B ON CAST(A.id AS SIGNED) >= B.id LIMIT 1");
    if(!success){
            m_db.rollback();
            return StatusCode::SQL_EXEC_ERROR;
    }
    success=query.next();
    if(!success){
            m_db.rollback();
            return StatusCode::SQL_EXEC_ERROR;
    }
    car_id=query.value(0).toString();
    QString insert_sql="INSERT car_info(model, payer_id, color, plate, type, car_id) VALUES ( \
            :model,:payer_id,:color,:plate,:car_type,:car_id);";

    query.prepare(insert_sql);
    query.bindValue(":model",car.model());
    query.bindValue(":payer_id",(car.payer_id())==""?NULL:car.payer_id());
    query.bindValue(":color",car.color());
    query.bindValue(":plate",car.plate());
    query.bindValue(":car_type",car.type());
    query.bindValue(":car_id",car_id);

    success=query.exec();
    if(!success){
            m_db.rollback();
            return StatusCode::SQL_EXEC_ERROR;
    }

    query.prepare(" DELETE FROM id_contain WHERE id = :car_id");
    query.bindValue(":car_id",car_id);

    success=query.exec();
    if(!success){
            m_db.rollback();
            return StatusCode::SQL_EXEC_ERROR;
    }
    m_db.commit();


    // add to redis
    car.setCar_id(car_id);
    m_rd.beginHset("car_"+car_id.toStdString(),"model",car.model().toStdString());
    m_rd.beginHset("car_"+car_id.toStdString(),"payer_id",car.payer_id().toStdString());
    m_rd.beginHset("car_"+car_id.toStdString(),"color",car.color().toStdString());
    m_rd.beginHset("car_"+car_id.toStdString(),"plate",car.plate().toStdString());
    m_rd.beginHset("car_"+car_id.toStdString(),"car_type",std::to_string( car.type()));
    m_rd.beginHset("car_"+car_id.toStdString(),"car_id",car.car_id().toStdString());

    m_rd.set("plate_"+car.plate().toStdString(),car_id.toStdString());
    m_rd.endCommand();
    //add to redis end

    return StatusCode::SUCCESS;

}


int CCarDao::bundleAndUnboundle(const QString &payer_id, const QString &car_id)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE car_info SET payer_id=:payer_id where car_id=:car_id;");
    query.bindValue(":car_id",car_id);
    query.bindValue(":payer_id",payer_id);
    bool success;
    success=query.exec();
    if(!success){
        return StatusCode::SQL_EXEC_ERROR;
    }

    // add to redis
    if(payer_id==NULL){
        m_rd.beginHset("car_"+car_id.toStdString(),"payer_id","");
    }else{
        m_rd.beginHset("car_"+car_id.toStdString(),"payer_id",payer_id.toStdString());
    }
    m_rd.endCommand();
    // add to redis end

    return StatusCode::SUCCESS;
}

int CCarDao::del_car(const QString &car_id)
{
    QSqlQuery query(m_db);
    m_db.transaction();
    query.prepare("delete from car_info where car_id=:car_id;");
    query.bindValue(":car_id",car_id);

    if(!query.exec()){
        m_db.rollback();
        return StatusCode::SQL_EXEC_ERROR;
    }

    query.prepare("INSERT INTO id_contain(id) VALUES(:id);");
    query.bindValue(":id",car_id);
    if(!query.exec()){
        m_db.rollback();
        return StatusCode::SQL_EXEC_ERROR;
    }
    m_db.commit();


    std::basic_string<char> plate= m_rd.hget("car_"+car_id.toStdString(),"plate");
    m_rd.del("car_"+car_id.toStdString());
    m_rd.del("plate_"+plate);

    return StatusCode::SUCCESS;
}

int CCarDao::update_car(const CCar &car)
{
    QSqlQuery query(m_db);

    QString update_sql="UPDATE car_info SET model=:model,payer_id=:payer_id,color=:color,plate=:plate,type=:type  where car_id=:car_id;";
    query.prepare(update_sql);

    query.bindValue(":model",car.model());
    query.bindValue(":payer_id",(car.payer_id()==""?NULL:car.payer_id()));
    query.bindValue(":color",car.color());
    query.bindValue(":plate",car.plate());
    query.bindValue(":type",car.type());
    query.bindValue(":car_id",car.car_id());

    QString car_id=car.car_id();
    bool success=query.exec();
    if(!success){
            return StatusCode::SQL_EXEC_ERROR;
    }
    m_rd.beginHset("car_"+car_id.toStdString(),"model",car.model().toStdString());
    m_rd.beginHset("car_"+car_id.toStdString(),"payer_id",car.payer_id().toStdString());
    m_rd.beginHset("car_"+car_id.toStdString(),"color",car.color().toStdString());
    m_rd.beginHset("car_"+car_id.toStdString(),"plate",car.plate().toStdString());
    m_rd.beginHset("car_"+car_id.toStdString(),"car_type",std::to_string( car.type()));
    m_rd.beginHset("car_"+car_id.toStdString(),"car_id",car.car_id().toStdString());
    m_rd.set("plate_"+car.plate().toStdString(),car_id.toStdString());

    m_rd.endCommand();
    return StatusCode::SUCCESS;

}

int CCarDao::query_cars_by_payer(QList<CCar> &cars, const QString &payer_id)
{

    QSqlQuery query(m_db);
    query.prepare("select * from car_info where payer_id=:payer_id;");
    query.bindValue(":payer_id",payer_id);
    bool success=query.exec();
    if(!success){
        return StatusCode::SQL_EXEC_ERROR;
    }
    while(query.next()){
        QString m_plate=query.value("plate").toString();
        QString m_car_id=query.value("car_id").toString();
        QString m_color=query.value("color").toString();
        QString m_model=query.value("model").toString();
        QString m_payer_id=query.value("payer_id").toString();
        int m_type=query.value("type").toInt();
        cars.append(CCar(m_plate,m_car_id,m_color,m_model,m_payer_id,m_type));
    }
    return StatusCode::SUCCESS;

}

int CCarDao::query_car_by_plate(CCar &car, const QString &plate)
{
    if(m_rd.exists("plate_"+plate.toStdString())){
        QString m_car_id=QString::fromStdString(m_rd.get("plate_"+plate.toStdString()));
        if(m_rd.exists("car_"+m_car_id.toStdString())){
            car.setCar_id(m_car_id);
            QString m_color=QString::fromStdString(m_rd.hget("car_"+m_car_id.toStdString(),"color"));
            car.setColor(m_color);
            QString m_model=QString::fromStdString(m_rd.hget("car_"+m_car_id.toStdString(),"model"));
            car.setModel(m_model);
            QString m_payer_id=QString::fromStdString( m_rd.hget("car_"+m_car_id.toStdString(),"payer_id"));
            car.setPayer_id(m_payer_id);
            int m_type=std::stoi(m_rd.hget("car_"+m_car_id.toStdString(),"car_type"));
            car.setType(m_type);
            car.setPlate(plate);
            return StatusCode::SUCCESS;
        }
    }

    QSqlQuery query(m_db);
    QString sql="select * from car_info where plate=:plate;";
    query.prepare(sql);
    query.bindValue(":plate",plate);

    bool success=query.exec();
    if(!success){
        return StatusCode::SQL_EXEC_ERROR;
    }

    if(query.next()){
        QString m_plate=query.value("plate").toString();
        car.setPlate(m_plate);
        QString m_car_id=query.value("car_id").toString();
        car.setCar_id(m_car_id);
        QString m_color=query.value("color").toString();
        car.setColor(m_color);
        QString m_model=query.value("model").toString();
        car.setModel(m_model);
        QString m_payer_id=query.value("payer_id").toString();
        car.setPayer_id(m_payer_id);
        int m_type=query.value("type").toInt();
        car.setType(m_type);
    }else{
        return  StatusCode::EMPTY_QUERY;
    }

    QString car_id=car.car_id();
    m_rd.beginHset("car_"+car_id.toStdString(),"model",car.model().toStdString());
    m_rd.beginHset("car_"+car_id.toStdString(),"payer_id",car.payer_id().toStdString());
    m_rd.beginHset("car_"+car_id.toStdString(),"color",car.color().toStdString());
    m_rd.beginHset("car_"+car_id.toStdString(),"plate",car.plate().toStdString());
    m_rd.beginHset("car_"+car_id.toStdString(),"car_type",std::to_string( car.type()));
    m_rd.beginHset("car_"+car_id.toStdString(),"car_id",car.car_id().toStdString());
    m_rd.set("plate_"+car.plate().toStdString(),car_id.toStdString());
    m_rd.endCommand();

    return StatusCode::SUCCESS;
}

int CCarDao::query_cars(std::vector<CCar> &cars, int &all_results_num, const QString &keyword, int page, int page_size)
{

    QSqlQuery query(m_db);
    int offset=(page-1)*page_size;
    QString sql="SELECT SQL_CALC_FOUND_ROWS * FROM car_info \
            WHERE CONCAT(IFNULL(plate,''),IFNULL(payer_id,'')) LIKE :keyword \
            order by car_id LIMIT :offset,:page_size;";

    query.prepare(sql);

    query.bindValue(":keyword","%"+keyword+"%");
    query.bindValue(":offset",offset);
    query.bindValue(":page_size",page_size);

    if(query.exec()){
        while(query.next()){
            CCar car;
            QString m_plate=query.value("plate").toString();
            car.setPlate(m_plate);
            QString m_car_id=query.value("car_id").toString();
            car.setCar_id(m_car_id);
            QString m_color=query.value("color").toString();
            car.setColor(m_color);
            QString m_model=query.value("model").toString();
            car.setModel(m_model);
            QString m_payer_id=query.value("payer_id").toString();
            car.setPayer_id(m_payer_id);
            int m_type=query.value("type").toInt();
            car.setType(m_type);

            cars.push_back(car);
        }

        query.exec("SELECT FOUND_ROWS()");
        query.next();
        all_results_num=query.value(0).toInt();


        if(cars.size()==0){
            return StatusCode::EMPTY_QUERY;
        }
        return StatusCode::SUCCESS;

    }else{
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError().text();
        return StatusCode::SQL_EXEC_ERROR;
    }
}
