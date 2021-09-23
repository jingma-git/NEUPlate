#include "server_handler.h"

#include <QDebug>
#include "request_handler.h"

/**
* @functionName  MainServer
* @Description   class MainServer's constructor
* @author        chenhanlin
* @date          2018-07-01
* @parameter     int port, QObject *parent
*/
CServerHandler::CServerHandler(int port, QObject *parent) : QObject(parent), m_server(new QTcpServer(this))
{
    // listen
    if (!this->m_server->listen(QHostAddress::AnyIPv4, port)){
        qWarning() << "Server cannot listen at 0.0.0.0 :" << port;
        exit(1);
    }
    qDebug() << "Server start listen at 0.0.0.0 :" << port;
    // bind newconnection with handle_new_connection slot
    connect(m_server, SIGNAL(newConnection()), this, SLOT(handle_new_connection()));
}

CServerHandler::~CServerHandler()
{
    m_server->close();
    delete m_server;
}

void CServerHandler::handle_new_connection()
{
    QTcpSocket *p_socket = m_server->nextPendingConnection(); // accept connect
    CRequestHandler *handler = new CRequestHandler(p_socket, this); // pass to handler
    qDebug() << "a new connection from " << handler->client_name();
}
