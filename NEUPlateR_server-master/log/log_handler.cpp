#include "log_handler.h"


QMutex LogHandler::m_mutex;
QFile *LogHandler::m_log_file = NULL;
QTextStream *LogHandler::m_file_out = NULL;
int LogHandler::m_flag = 0;

/**
* @functionName  LogHandler
* @Description   class LogHandler constructor that init log handle resource.
* @author        luxijia
* @date          2018-7-23
*/
LogHandler::LogHandler()
{
    QSettings setting(":/app.ini", QSettings::IniFormat);

    m_log_dir.setPath("logging");

    QString log_file_path = m_log_dir.absoluteFilePath("log.txt");

    m_create_file_date = QFileInfo(log_file_path).created().date();
    open_and_backup_file();

    /*
    m_rename_log_file_timer.setInterval(1000 * 60 * 5);
    m_rename_log_file_timer.start();
    QObject::connect(&m_rename_log_file_timer, &QTimer::timeout,
            [this]{
                QMutexLocker locker(&m_mutex);
                open_and_backup_file();
    });

    m_flush_log_file_timer.setInterval(1000);
    m_flush_log_file_timer.start();
    QObject::connect(&m_flush_log_file_timer, &QTimer::timeout,
            [this]{
                QMutexLocker locker(&m_mutex);

                if (NULL != m_file_out)
                    m_file_out->flush();
    });
    */
}

/**
* @functionName  ~LogHandler
* @Description   class LogHandler deconstructor that release clas LogHandler resource.
* @author        luxijia
* @date          2018-7-23
*/
LogHandler::~LogHandler()
{

    if (NULL != m_log_file)
    {
        m_log_file->flush();
        m_log_file->close();

        delete m_file_out;
        delete m_log_file;

        m_file_out = NULL;
        m_log_file = NULL;
    }
}

/**
* @functionName  get_instance
* @Description   use Single pattern to get class LogHandler object
* @author        luxijia
* @date          2018-6-30
* @parameter
* @return        ConnectionPool&
*/
LogHandler &LogHandler::get_instance()
{
    QMutexLocker locker(&m_mutex);
    static LogHandler log_instance;

    return log_instance;
}

/**
* @functionName  open_and_backup_file
* @Description   if no log file then create a new log file.
*                if has a log file then if file create day is no today, then save file
*                and create a new file to log message.
* @author        luxijia
* @date          2018-7-23
*/
void LogHandler::open_and_backup_file()
{
    //create lof dir if no exist
    if (!m_log_dir.exists())
        m_log_dir.mkpath(".");

    QString log_file_path = m_log_dir.absoluteFilePath("log.txt");

    if (NULL == m_log_file)
    {
        m_log_file = new QFile(log_file_path);
        m_file_out = (m_log_file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) ? new QTextStream(m_log_file) : NULL;

        //set file code is utf-8
        if (NULL != m_file_out)
            m_file_out->setCodec("UTF-8");

        if (m_create_file_date.isNull())
            m_create_file_date = QDate::currentDate();
    }

    if (m_create_file_date != QDate::currentDate())
    {
        m_log_file->flush();
        m_log_file->close();
        delete m_file_out;
        delete m_log_file;

        QString new_path = m_log_dir.absoluteFilePath(m_create_file_date.toString("yyyy-MM-dd.log"));
        QFile::copy(log_file_path, new_path);
        QFile::remove(log_file_path);

        m_log_file = new QFile(log_file_path);
        m_file_out = (m_log_file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) ? new QTextStream(m_log_file) : NULL;
        m_create_file_date = QDate::currentDate();

        if (NULL != m_file_out)
            m_file_out->setCodec("UTF-8");
    }
}

/**
* @functionName  flush_log_file
* @brief         flush buffer into file
* @author        chenhanlin
* @date          2018-07-26
* @parameter     void
* @return        void
*/
void LogHandler::flush_log_file()
{
    if (NULL != m_file_out)
        m_file_out->flush();
}

/**
* @functionName  message_handle
* @Description   handle log message, recogenize meassage type, time, context.
* @author        luxijia
* @date          2018-7-23
* @parameter     type call parameter type
* @parameter     context log message position context
* @parameter     msg log message context
*/
void LogHandler::message_handle(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&m_mutex);

    //log header
    QString text;

    //log has four message type
    switch (type) {

    case QtDebugMsg:
        text = QString("[Debug]:");
        break;

    case QtWarningMsg:
        text = QString("[Waring]:");
        break;

    case QtCriticalMsg:
        text = QString("[Critical]:");
        break;

    case QtFatalMsg:
        text = QString("[Fatal]:");
        break;
    }

    //log file context define
    //error message generate position
//    QString position = QString("File:(%1) Line(%2)").arg(context.file).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %3 %4").arg(current_date).arg(text).arg(msg);

    (*m_file_out) << message << "\n";
    std::cout << std::string(message.toLocal8Bit()) << std::endl;
}

/**
* @functionName  install_message_handler
* @Description   install log message handle funtion to QT log model.
* @author        luxijia
* @date          2018-7-23
*/
void LogHandler::install_message_handler()
{
    QMutexLocker locker(&m_mutex);

    if (0 == m_flag)
    {
        m_flag = 1;
        qInstallMessageHandler(LogHandler::message_handle);
    }
}

/**
* @functionName  release
* @Description   cancel log message function connect with QT log model.
* @author        luxijia
* @date          2018-7-23
*/
void LogHandler::release()
{
    QMutexLocker locker(&m_mutex);
    qInstallMessageHandler(0);
    m_flag = 0;
}

/**
* @functionName  debug
* @Description   show debug log message to console and write to log file.
* @author        luxijia
* @date          2018-7-24
*/
void Log::debug(const QString &str)
{
    qDebug() << qPrintable(str);
}

/**
* @functionName  info
* @Description   show info log message to console and write to log file.
* @author        luxijia
* @date          2018-7-24
*/
void Log::info(const QString &str)
{
    qDebug() << qPrintable(str);
}

/**
* @functionName  warning
* @Description   show warning log message to console and write to log file.
* @author        luxijia
* @date          2018-7-24
*/
void Log::warning(const QString &str)
{
    qWarning() << qPrintable(str);
}

/**
* @functionName  fatal
* @Description   show fatal log message to console and write to log file.
* @author        luxijia
* @date          2018-7-24
*/
void Log::fatal(const QString &str)
{
    qWarning() << qPrintable(str);
}

/**
* @functionName  error
* @Description   show error log message to console and write to log file.
* @author        luxijia
* @date          2018-7-24
*/
void Log::error(const QString &str)
{
    qWarning() << qPrintable(str);
}
