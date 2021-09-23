/**
* @projectName   NEUPlateR_server
* @brief         add log module, write log in console and file
* @author        chenhanlin
* @date          2018-07-20
* @modify_author chenhanlin
* @modify_date   2018-07-20
*/
#ifndef LOG_HANDLER_H
#define LOG_HANDLER_H

/**
* @projectName   LogHandler
* @brief         Define a class Connection implements that
*                handle log message.
* @author        luxijia
* @date          2018-7-23
* @modify_author
* @modify_date
*/

#include <QDir>
#include <QDebug>
#include <QtDebug>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QSettings>
#include <QTextStream>
#include <QTimer>
#include <iostream>
#include <functional>


namespace Log {
void debug(const QString &str);
void info(const QString &str);
void warning(const QString &str);
void error(const QString &str);
void fatal(const QString &str);
}

class LogHandler
{
public:
    static LogHandler& get_instance();
    void install_message_handler();
    void release();
    ~LogHandler();
    void open_and_backup_file();
    void flush_log_file();
private:
    LogHandler();
    LogHandler(const LogHandler& handler);
    LogHandler &operator=(const LogHandler &handler);

    static void message_handle(QtMsgType type, const QMessageLogContext& context, const QString &msg);
private:
    QDir m_log_dir;
//    QTimer m_rename_log_file_timer;
//    QTimer m_flush_log_file_timer;
    QDate m_create_file_date;

    static QFile *m_log_file;
    static QTextStream *m_file_out;
    static QMutex m_mutex;
    static int m_flag;
};

#endif // LOG_HANDLER_H
