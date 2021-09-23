/**
* @projectName   Connection
* @brief         Implement the funtion of class Connection
* @author        luxijia
* @date          2018-6-29
* @see           Connection
* @modify_author
* @modify_date
*/

#include "connection_pool.h"

#include <QFile>
#include <QDir>
#include "neu_head.h"
QMutex ConnectionPool::m_mutex;
QWaitCondition ConnectionPool::m_wait_condition;

/**
* @functionName  ConnectionPool
* @Description   class ConnectionPool constructor
* @author        luxijia
* @date          2018-7-23
*/
ConnectionPool::ConnectionPool()
{
    QSettings setting(":/app.ini", QSettings::IniFormat);
    setting.beginGroup("database");
    m_host_name = setting.value("host_name").toString();
    m_database_name = setting.value("database_name").toString();
    m_user_name = setting.value("user_name").toString();
    m_password = setting.value("password").toString();
    m_database_type = QString("Q%1").arg(setting.value("database_type").toString());

    m_relink = true;
    m_test_sql = "SELECT 1";

    m_max_wait_time = setting.value("max_wait_time").toInt();
    m_wait_interval = setting.value("wait_interval").toInt();
    m_max_connection_count = setting.value("max_connection_count").toInt();
    m_init_connection_count = setting.value("init_connection_count").toInt();
    setting.endGroup();

    for (int i = 0; i < m_max_connection_count; i++)
        m_connection_name_pool.enqueue(QString("Connection-%1").arg(i + 1));

    for (int i = 0; i < m_init_connection_count; i++)
    {
        QString name = m_connection_name_pool.dequeue();
        QSqlDatabase db = create_connection(name);
        m_unused_connection_names.enqueue(name);
    }
    Log::debug("init database connection pool");

    /*
    m_remove_unuse_connection_timer.setInterval(1000 * 20);
    m_remove_unuse_connection_timer.start();
    QObject::connect(&m_remove_unuse_connection_timer, &QTimer::timeout,
                     [this]{
                        QMutexLocker locker(&m_mutex);

                        foreach (QString connection_name, m_unused_connection_names)
                        {
                            QSqlDatabase::removeDatabase(connection_name);
                            m_connection_name_pool.enqueue(connection_name);
                        }

                        m_unused_connection_names.clear();
    });
    */
}

/**
* @functionName  ~ConnectionPool
* @Description   class ConnectionPool deconstructor
* @author        luxijia
* @date          2018-7-23
*/
ConnectionPool::~ConnectionPool()
{
    foreach (QString connection_name, m_used_connection_names)
        QSqlDatabase::removeDatabase(connection_name);

    foreach (QString connection_name, m_unused_connection_names)
        QSqlDatabase::removeDatabase(connection_name);

}

/**
* @functionName  get_instance
* @Description   use Single pattern to get class ConnectionPool object
* @author        luxijia
* @date          2018-7-23
* @parameter
* @return        ConnectionPool&
*/
ConnectionPool& ConnectionPool::get_instance()
{
    static ConnectionPool instance;
    QMutexLocker locker(&m_mutex);
    return instance;
}

/**
* @functionName  release_free_connection
* @brief         release the database connection free now
* @author        chenhanlin
* @date          2018-07-26
* @parameter     void
* @return        void
*/
void ConnectionPool::release_free_connection()
{
    foreach (QString connection_name, m_unused_connection_names)
    {
        QSqlDatabase::removeDatabase(connection_name);
        m_connection_name_pool.enqueue(connection_name);
    }

    m_unused_connection_names.clear();
}

/**
* @functionName  create_connection
* @Description   create or reuse database connection
* @author        luxijia
* @date          2018-7-23
* @parameter     connection_name the name of QSqlDatabase connection
* @return        if create connection is vaild or has the same name connection if connection pool
*                return vaild QSqlDatabase object.
*/
QSqlDatabase ConnectionPool::create_connection(const QString &connection_name)
{
    //if connection has been create, use previous connection
    if (QSqlDatabase::contains(connection_name))
    {
        QSqlDatabase use_db = QSqlDatabase::database(connection_name);

        if (m_relink)
        {
            QSqlQuery query(m_test_sql, use_db);

            if (query.lastError().type() != QSqlError::NoError && !use_db.open())
            {
                qWarning() << "Open database error: " << use_db.lastError().text() << ".";
                return QSqlDatabase();
            }
        }

        return use_db;
    }

    //create new connection
    QSqlDatabase db = QSqlDatabase::addDatabase(m_database_type, connection_name);
    db.setHostName(m_host_name);
    db.setDatabaseName(m_database_name);
    db.setUserName(m_user_name);
    db.setPassword(m_password);

    if (!db.open())
    {
        qWarning() << "Create database error: " << db.lastError().text() << ".";
        return QSqlDatabase();
    }

    return db;
}

/**
* @functionName  open_connection
* @Description   get a connection from ConnectionPool
* @author        luxijia
* @date          2018-6-30
* @parameter
* @return        if create connection is vaild or has the same name connection if connection pool
*                return vaild QSqlDatabase object.
*/
QSqlDatabase ConnectionPool::open_connection()
{
    ConnectionPool &conn = ConnectionPool::get_instance();
    QString connection_name;

    //add lock
    QMutexLocker locker(&m_mutex);

    //count of connection
    int connection_count = conn.m_used_connection_names.size() + conn.m_unused_connection_names.size();


    for (int i = 0; i < conn.m_max_wait_time && conn.m_unused_connection_names.size() == 0
         && connection_count == conn.m_max_connection_count ; i += conn.m_wait_interval)
    {
        m_wait_condition.wait(&m_mutex, conn.m_wait_interval);
        connection_count = conn.m_used_connection_names.size() + conn.m_unused_connection_names.size();
    }

    if (conn.m_unused_connection_names.size() > 0)
        connection_name = conn.m_unused_connection_names.dequeue();
    else if (connection_count < conn.m_max_connection_count)
        connection_name = conn.m_connection_name_pool.dequeue();
    else
    {
        qWarning() << "Cannot create more connections.";
        return QSqlDatabase();
    }

    QSqlDatabase db = conn.create_connection(connection_name);

    if (db.isOpen())
        conn.m_used_connection_names.enqueue(connection_name);

    return db;
}

/**
* @functionName  close_connection
* @Description   return a connection back connection pool
* @author        luxijia
* @date          2018-7-23
* @parameter     connection using connection
* @return
*/
void ConnectionPool::close_connection(QSqlDatabase connection)
{
    ConnectionPool &conn = ConnectionPool::get_instance();
    QString connection_name = connection.connectionName();

    QMutexLocker locker(&m_mutex);

    if (conn.m_used_connection_names.contains(connection_name))
    {
        conn.m_used_connection_names.removeOne(connection_name);
        conn.m_unused_connection_names.enqueue(connection_name);
        m_wait_condition.wakeOne();
    }
}

///**
//* @functionName  release
//* @Description   close all connection in connection pool
//* @author        luxijia
//* @date          2018-7-23
//*/
//void ConnectionPool::release()
//{
//    delete this;
//}

