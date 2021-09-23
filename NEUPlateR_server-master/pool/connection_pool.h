#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

/**
* @projectName   ConnectionPool
* @brief         Define a class Connection implements that
*                mysql connecton pool for database's operation.
* @author        luxijia
* @date          2018-7-23
* @modify_author
* @modify_date
*/

#include <QSql>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QQueue>
#include <QString>
#include <QMutex>
#include <QTimer>
#include <QWaitCondition>
#include <QMutexLocker>
#include <QSettings>

class ConnectionPool
{
public:
//    static void release(); //close all connection in connection pool
    static QSqlDatabase open_connection();//get a connection from connection pool
    static void close_connection(QSqlDatabase connection);//return connection to connection pool
    static ConnectionPool& get_instance();
    void release_free_connection();
    ~ConnectionPool();

private:
    ConnectionPool();
    ConnectionPool(const ConnectionPool &conn);
    ConnectionPool& operator=(const ConnectionPool &conn);
    QSqlDatabase create_connection(const QString &connectionName);

    QQueue<QString> m_used_connection_names;//the name of use database connection
    QQueue<QString> m_unused_connection_names;//the name of unuse database connection
    QQueue<QString> m_connection_name_pool;

    QString m_host_name;
    QString m_database_name;
    QString m_user_name;
    QString m_password;
    QString m_database_type;
//    QTimer m_remove_unuse_connection_timer;

    bool m_relink;//whether relink after connection break
    QString m_test_sql;//verif that the connection is vaild by test sql

    int m_max_wait_time;//the maximum wait time of getting connection
    int m_wait_interval;//the interval wait time of trying to getconnection
    int m_init_connection_count;//initial connection count
    int m_max_connection_count;//maximun connection count

    static QMutex m_mutex;
    static QWaitCondition m_wait_condition;
};

#endif // CONNECTIONPOOL_H
