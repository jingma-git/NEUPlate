#include "request_handler.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QStringList>
#include "request.h"
#include "Route/action.h"
#include "Demo/say_hello_action.h"

CRequestHandler::CRequestHandler(QTcpSocket *p_socket, QObject *parent)
    : QObject(parent),
      m_p_socket(p_socket),
      m_client_name(p_socket->peerAddress().toString())
{
    qRegisterMetaType<CResponse>("Response");
    connect(m_p_socket, SIGNAL(readyRead()), this, SLOT(message_arrive()));
    connect(m_p_socket, SIGNAL(disconnected()), this, SLOT(connection_close()));
}

CRequestHandler::~CRequestHandler()
{
    delete m_p_socket;
}

QString CRequestHandler::client_name() const
{
    return m_client_name;
}

QThreadPool &CRequestHandler::pool()
{
    static QThreadPool thread_pool;
    static bool init = false;
    if(!init){
        thread_pool.setMaxThreadCount(200);
        init = true;
    }
    return thread_pool;
}

/**
* @functionName  connection_close
* @Description   when connection close, it should release some resource
* @author        chenhanlin
* @date          2018-07-01
* @parameter     void
* @return        void
*/
void CRequestHandler::connection_close()
{
    qDebug() << "close connection with " << m_client_name;
    m_p_socket->close();
//    UserController::logout(this->m_i_handler_id);
    deleteLater();
}

/**
* @functionName  message_arrive
* @Description   when message arrived, it will receive data, handle data and send response
* @author        chenhanlin
* @date          2018-07-01
* @parameter     void
* @return        void
*/
void CRequestHandler::message_arrive()
{
    cache += m_p_socket->readAll();
    QStringList reqs = cache.split("\r\n\r\n");
    // handle request after read all data
    if(!cache.endsWith("\r\n\r\n")){
        cache = reqs[reqs.size() - 1];
        for(int i=0; i<reqs.size()-1; i++){
            m_req_queue.push(reqs[i]);
        }
    }else{
        cache.clear();
        for(int i=0; i<reqs.size(); i++){
            m_req_queue.push(reqs[i]);
        }
    }

    //
    while(!m_req_queue.empty()){
        // get request data
        QString message(m_req_queue.front());
        m_req_queue.pop();
        if(message.isEmpty()) continue;
        // translate request data
        QByteArray data = message.toUtf8();
        QJsonDocument json_doc(QJsonDocument::fromJson(data));
        QJsonObject json(json_doc.object());
        CRequest *p_req = new CRequest(json);

        qDebug() << "receive task from " << m_client_name << ", to do " << p_req->action();

        CAction *p_action = CAction::create(p_req->action(), p_req);
        if(nullptr == p_action){
            continue;
        }
//        connect(p_action, SIGNAL(notify_send(CResponse)), this, SLOT(send_message(CResponse)));
//        p_action->setAutoDelete(true);
//        pool().start(p_action);
    }
}

/**
* @functionName  send_message
* @Description   send response message to server
* @author        chenhanlin
* @date          2018-07-19
* @parameter     Response resp
* @return        void
*/
void CRequestHandler::send_message(CResponse resp)
{
    QString send_json(resp.to_json());
    m_p_socket->write(send_json.toUtf8());
}
