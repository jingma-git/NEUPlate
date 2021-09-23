#ifndef MAINSERVER_H
#define MAINSERVER_H

#include <QObject>
#include <QtNetwork>

class CServerHandler : public QObject
{
    Q_OBJECT
public:
    explicit CServerHandler(int port, QObject *parent = nullptr);
    ~CServerHandler();

private:
    QTcpServer *m_server;

public slots:
    void handle_new_connection();
};

#endif // MAINSERVER_H
