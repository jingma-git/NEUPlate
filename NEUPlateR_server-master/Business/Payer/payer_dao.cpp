#include "payer_dao.h"

CPayerDao::CPayerDao():
    m_rd("localhost")
{
    this->db_from_out=false;
}

CPayerDao::CPayerDao(const QSqlDatabase &db):m_db(db), m_rd("localhost")
{
    this->db_from_out=true;
}

CPayerDao::~CPayerDao()
{
    if(this->db_from_out==false){
    ConnectionPool::close_connection(m_db);
    }
}

int CPayerDao::query_payers(std::vector<CPayer> &payers, int &all_results_num,const QString &keyword,int page,int page_size)
{

    open_db();
    QSqlQuery query(m_db);
    int offset=(page-1)*page_size;
    QString sql="SELECT SQL_CALC_FOUND_ROWS * FROM payer_info \
            WHERE CONCAT(IFNULL(name,''),IFNULL(id_card,''),IFNULL(username,'')) LIKE :keyword \
            order by payer_id LIMIT :offset,:page_size";
    bool success;
    query.prepare(sql);
    query.bindValue(":keyword","%"+keyword+"%");
    query.bindValue(":offset",offset);
    query.bindValue(":page_size",page_size);
    success=query.exec();
    if(!success){
            return StatusCode::SQL_EXEC_ERROR;
    }
    while(query.next()){
        CPayer cur_payer;
        QString name=query.value("name").toString();
        cur_payer.setName(name);
        QString tel=query.value("tel").toString();
        cur_payer.setTel(tel);
        QString payer_id=query.value("payer_id").toString();
        cur_payer.setPayer_id(payer_id);
        double balance=query.value("balance").toDouble();
        cur_payer.setBalance(balance);
        QString password=query.value("password").toString();
        cur_payer.setPassword(password);
        QString username=query.value("username").toString();
        cur_payer.setUsername(username);
        QString id_card=query.value("id_card").toString();
        cur_payer.setId_card(id_card);
        QString birthday=query.value("birthday").toString();
        cur_payer.setBirthday(birthday);
        int gender=query.value("gender").toInt();
        cur_payer.setGender(gender);
        QString photo=query.value("photo").toString();
        cur_payer.setPhoto(photo);
        payers.push_back(cur_payer);
    }

    if(payers.size()==0){
        all_results_num=0;
        return StatusCode::EMPTY_QUERY;
    }else{
        query.exec("SELECT FOUND_ROWS()");
        query.next();
        all_results_num=query.value(0).toInt();
        return StatusCode::SUCCESS;
    }
}

int CPayerDao::add_payer(CPayer &payer)
{
    open_db();
    QSqlQuery query(m_db);
    m_db.transaction();
    QString payer_id;
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
    payer_id=query.value(0).toString();

    QString insert_sql="INSERT INTO payer_info(name, tel, payer_id, balance,password, username, \
            id_card, birthday, gender, photo) VALUES ( \
            :name,:tel,:payer_id,:balance,:password,:user_name,:id_card,:birthday,:gender,:photo    \
                );";
    query.prepare(insert_sql);

    query.bindValue(":name",payer.name());
    query.bindValue(":tel",payer.tel());
    query.bindValue(":payer_id",payer_id);
    query.bindValue(":balance",payer.balance());
    query.bindValue(":password",payer.password());
    query.bindValue(":user_name",payer.username());
    query.bindValue(":id_card",payer.id_card());
    query.bindValue(":birthday",payer.birthday());
    query.bindValue(":photo",payer.photo());
    query.bindValue(":gender",payer.gender());

    success=query.exec();
    if(!success){
            m_db.rollback();
            return StatusCode::SQL_EXEC_ERROR;
    }

    query.prepare(" DELETE FROM id_contain WHERE id = :payer_id");
    query.bindValue(":payer_id",payer_id);

    success=query.exec();
    if(!success){
            m_db.rollback();
            return StatusCode::SQL_EXEC_ERROR;
    }

    m_db.commit();
    payer.setPayer_id(payer_id);

    add_payer_to_redis(payer);

    return StatusCode::SUCCESS;
}

int CPayerDao::update_payer(const CPayer &payer)
{
    open_db();
    QSqlQuery query(m_db);

    QString update_sql="UPDATE payer_info SET name=:name, tel=:tel, balance=:balance, password=:password,\
            username=:user_name, birthday=:birthday, gender=:gender, photo=:photo where payer_id=:payer_id;";
    query.prepare(update_sql);

    query.bindValue(":name",payer.name());
    query.bindValue(":tel",payer.tel());
    query.bindValue(":payer_id",payer.payer_id());
    query.bindValue(":balance",payer.balance());
    query.bindValue(":password",payer.password());
    query.bindValue(":user_name",payer.username());
    query.bindValue(":id_card",payer.id_card());
    query.bindValue(":birthday",payer.birthday());
    query.bindValue(":photo",payer.photo());
    query.bindValue(":gender",payer.gender());

    bool success=query.exec();
    if(!success){
            return StatusCode::SQL_EXEC_ERROR;
    }

    add_payer_to_redis(payer);
    return StatusCode::SUCCESS;
}

int CPayerDao::add_balance(const QString &id_card, const double &value)
{
    const double max_balance=999999.99;
    open_db();
    QSqlQuery query(m_db);

    double balance;
    int result=query_balance(id_card,balance);
    if(StatusCode::SUCCESS!=result){
        return result;
    }
    if(balance+value<0){
        return StatusCode::BALANCE_NEGATIVE;
    }else if(balance+value>max_balance){
        return StatusCode::BALANCE_OVERFLOW;
    }

    query.prepare("UPDATE payer_info SET balance=:balance where id_card=:id_card;");
    query.bindValue(":balance",balance+value);
    query.bindValue(":id_card",id_card);

    if(!query.exec()){
        return StatusCode::SQL_EXEC_ERROR;
    }

    m_rd.set("id_card_"+id_card.toStdString(),std::to_string(balance+value));


    return StatusCode::SUCCESS;
}

int CPayerDao::add_balance_by_payer_id(const QString &payer_id, const double &value)
{
    open_db();
    CPayer payer;
    int result=query_payer_by_id(payer,payer_id);

    if(result!=StatusCode::SUCCESS){
        return result;
    }else{
        result=add_balance(payer.id_card(),value);
        if(result!=StatusCode::SUCCESS){
            return result;
        }else{
            return result;
        }
    }
}

int CPayerDao::query_balance(const QString &id_card, double &balance)
{

    //从redis里面查余额信息。
    if(m_rd.exists("id_card_"+id_card.toStdString())){
       balance= std::stod(m_rd.get("id_card_"+id_card.toStdString()));
       return StatusCode::SUCCESS;
    }

        open_db();
       QSqlQuery query(m_db);
       query.prepare("SELECT balance FROM payer_info WHERE id_card=:id_card;");
       query.bindValue(":id_card",id_card);

       int success;
       success=query.exec();
       if(!success){
           return StatusCode::SQL_EXEC_ERROR;
       }
       success=query.next();
       if(!success){
           return StatusCode::PAYER_NOT_FOUND;
       }
       balance = query.value(0).toDouble();

       //将余额信息存到redis里面
       m_rd.set("id_card_"+id_card.toStdString(),std::to_string(balance));


       return StatusCode::SUCCESS;
}

int CPayerDao::quert_balance_by_payer_id(const QString &payer_id, double &balance)
{
    open_db();
    CPayer payer;
    int result=query_payer_by_id(payer,payer_id);

    if(result!=StatusCode::SUCCESS){
        return result;
    }else{
        balance=payer.balance();
        return result;
    }
}


int CPayerDao::valid_payer(const QString &user_name, const QString &password, bool &result)
{
    open_db();
    QSqlQuery query(m_db);

    query.prepare("SELECT count(*)  FROM payer_info WHERE username=:user_name AND password=:password");
    query.bindValue(":user_name",user_name);
    query.bindValue(":password",password);

    if(!query.exec()){
        return StatusCode::SQL_EXEC_ERROR;
    }
    query.next();
    int num=query.value(0).toInt();
    if(num==1){
        result=true;
    }else{
        result=false;
    }
    return StatusCode::SUCCESS;
}


/**
 * @brief CPayerDao::del_payer delete payer info ，Unbound  cars
 * @param payer_id
 * @return
 */
int CPayerDao::del_payer(const QString &payer_id)
{
    open_db();
    QSqlQuery query(m_db);
    m_db.transaction();
    bool result;
    QString del_sql="DELETE FROM payer_info WHERE  payer_id=:payer_id";
    query.prepare(del_sql);
    query.bindValue(":payer_id",payer_id);
    result = query.exec();
    if(!result){
        m_db.rollback();
        return StatusCode::SQL_EXEC_ERROR;
    }

    query.prepare("INSERT INTO id_contain(id) VALUES(:id);");
    query.bindValue(":id",payer_id);

    result=query.exec();
    if(!result){
        m_db.rollback();
        return StatusCode::SQL_EXEC_ERROR;
    }
    m_db.commit();

    if(m_rd.exists("payer_"+payer_id.toStdString())){
        QString id_card= QString::fromStdString(m_rd.hget("payer_"+payer_id.toStdString(),"id_card"));
        m_rd.del(id_card.toStdString());
    }

    m_rd.del("payer_"+payer_id.toStdString());

    return StatusCode::SUCCESS;

}



/**
返回值：
PAYER_NOT_FOUND 没找到那个用户
SQL_EXEC_ERROR sql执行错误
SUCCESS 成功
**/
int CPayerDao::query_payer_by_id(CPayer &payer, const QString &payer_id)
{
    if(m_rd.exists("payer_"+payer_id.toStdString())){
        get_payer_from_redis(payer,payer_id);
        return StatusCode::SUCCESS;
    }
    open_db();

    QSqlQuery query(m_db);

    query.prepare("SELECT *  FROM payer_info WHERE payer_id=:payer_id");
    query.bindValue(":payer_id",payer_id);

    if(!query.exec()){
        return StatusCode::SQL_EXEC_ERROR;
    }
    if(query.next()){

        QString m_payer_id=query.value("payer_id").toString();
        QString m_username=query.value("username").toString();
        QString m_name=query.value("name").toString();
        QString m_id_card=query.value("id_card").toString();
        QString m_tel=query.value("tel").toString();
        QString m_password=query.value("password").toString();
        QString m_photo=query.value("photo").toString();
        double m_balance=query.value("balance").toDouble();
        QString m_birthday=query.value("birthday").toString();
        int m_gender=query.value("gender").toInt();

        payer.setPayer_id(m_payer_id);
        payer.setUsername(m_username);
        payer.setName(m_name);
        payer.setId_card(m_id_card);
        payer.setTel(m_tel);
        payer.setPassword(m_password);
        payer.setPhoto(m_photo);
        payer.setBalance(m_balance);
        payer.setBirthday(m_birthday);
        payer.setGender(m_gender);
    }else{
        return StatusCode::PAYER_NOT_FOUND;
    }
    payer.setPayer_id(payer_id);

    add_payer_to_redis(payer);

    return StatusCode::SUCCESS;
}

void CPayerDao::open_db()
{
    if(!m_db.isOpen()){
        m_db=ConnectionPool::open_connection();
    }
}

int CPayerDao::add_payer_to_redis(CPayer payer)
{
    QString payer_id=payer.payer_id();
    m_rd.set("id_card_"+payer.id_card().toStdString(),std::to_string(payer.balance()));
    m_rd.hset("payer_"+payer_id.toStdString(),"payer_id",payer.payer_id().toStdString());
    m_rd.hset("payer_"+payer_id.toStdString(),"username",payer.username().toStdString());
    m_rd.hset("payer_"+payer_id.toStdString(),"name",payer.name().toStdString());
    m_rd.hset("payer_"+payer_id.toStdString(),"id_card",payer.id_card().toStdString());
    m_rd.hset("payer_"+payer_id.toStdString(),"tel",payer.tel().toStdString());
    m_rd.hset("payer_"+payer_id.toStdString(),"password",payer.password().toStdString());
    m_rd.hset("payer_"+payer_id.toStdString(),"photo",payer.photo().toStdString());
    m_rd.hset("payer_"+payer_id.toStdString(),"birthday",payer.birthday().toStdString());
    m_rd.hset("payer_"+payer_id.toStdString(),"gender",std::to_string(payer.gender()));
    return 0;
}

void CPayerDao::get_payer_from_redis(CPayer &payer, QString payer_id)
{
    payer.setPayer_id(payer_id);
    std::string payer_id_s = payer_id.toStdString();


    QString username = QString::fromStdString(m_rd.hget("payer_" + payer_id_s,"username"));

    payer.setUsername(username);
    QString name=QString::fromStdString(m_rd.hget("payer_"+payer_id_s,"name"));
    payer.setName(name);

    QString id_card=QString::fromStdString(m_rd.hget("payer_"+payer_id_s,"id_card"));
    payer.setId_card(id_card);

    QString tel=QString::fromStdString(m_rd.hget("payer_"+payer_id_s,"tel"));
    payer.setTel(tel);

    QString password=QString::fromStdString(m_rd.hget("payer_"+payer_id_s,"password"));
    payer.setPassword(password);

    QString photo=QString::fromStdString(m_rd.hget("payer_"+payer_id_s,"photo"));
    payer.setPhoto(photo);

    double balance;
    if(m_rd.exists("id_card_"+id_card.toStdString())){
        balance=std::stod(m_rd.get("id_card_"+id_card.toStdString()));

    }else{
        balance=-1;
    }
    payer.setBalance(balance);
    QString birthday=QString::fromStdString(m_rd.hget("payer_"+payer_id_s,"birthday"));
    payer.setBirthday(birthday);
    int gender=std::stoi(m_rd.hget("payer_"+payer_id_s,"gender"));
    payer.setGender(gender);
}



