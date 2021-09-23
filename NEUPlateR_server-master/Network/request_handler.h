#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include <QtNetwork>
#include <QThreadPool>
#include <queue>
#include "response.h"

class CRequestHandler : public QObject
{
    Q_OBJECT
public:
    explicit CRequestHandler(QTcpSocket *p_socket, QObject *parent = nullptr);
    ~CRequestHandler();

    QString client_name() const;

private:
    QTcpSocket *m_p_socket;
//    QString m_i_handler_id;
    QString m_client_name;
    QString cache;
    std::queue<QString> m_req_queue;
    static QThreadPool& pool();

private slots:
    void connection_close();
    void message_arrive();
    void send_message(CResponse);
};

#endif // REQUESTHANDLER_H
