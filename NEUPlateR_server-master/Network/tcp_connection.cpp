#include "tcp_connection.h"
#include <QString>
#include <boost/bind.hpp>
#include <QStringList>
#include <cstring>
#include <iostream>
#include <thread>
#include <QUuid>
#include "pool/taskpool.h"
#include "pool/asio_thread_pool.h"
#include "Network/request.h"
#include "Network/response.h"
#include "Route/action.h"
#include "neu_head.h"
#include "Business/Employee/employee_controller.h"

CTcpConnection::CTcpConnection(boost::asio::io_context &io_contenxt)
    :m_socket(io_contenxt),
      m_socket_id(QUuid::createUuid().toString())
{
}

CTcpConnection::~CTcpConnection()
{
    if(!m_address.isEmpty())
        Log::debug(QString("Disconnected from ") + m_address);
    Employee::logout(m_socket_id);
}

void CTcpConnection::do_write(const std::string &data)
{
    boost::system::error_code ec;
    if(m_socket.is_open())
        this->m_socket.write_some(Asio::buffer(data+"\r\n\r\n"), ec);
    if(ec){
        handle_error(ec);
    }
}

/**
* @functionName  start
* @brief         start to use connection
* @author        chenhanlin
* @date          2018-07-25
* @parameter     void
* @return        void
*/
void CTcpConnection::start()
{
    m_address = QString::fromStdString(m_socket.remote_endpoint().address().to_string());
    Log::debug(QString("Establish a connection with ") + m_address);
    this->do_read();
}

/**
* @functionName  do_read
* @brief         async read data from socket
* @author        chenhanlin
* @date          2018-07-25
* @parameter     void
* @return        void
*/
void CTcpConnection::do_read()
{
    auto self = shared_from_this();
    m_socket.async_read_some(Asio::buffer(buf_read, buf_size - 1),
                             [this, self](boost::system::error_code ec, std::size_t size)
    {
        if(!ec){
            handle_read(ec, size);
            this->do_read();
        }else if(boost::asio::error::eof == ec){
            return;
        }else{
            this->handle_error(ec);
        }
    });
}

void CTcpConnection::handle_error(boost::system::error_code ec)
{
//    Log::debug(ec.message().c_str());
}

/**
* @functionName  handle_read
* @brief         processing read data
* @author        chenhanlin
* @date          2018-07-25
* @parameter     boost::system::error_code ec error code
* 				 std::size_t read_size the buf size system read
* @return        void
*/
void CTcpConnection::handle_read(boost::system::error_code ec, std::size_t read_size)
{
    if(ec || !read_size){
        return;
    }
    buf_read[read_size] = '\0';
    QString buf(m_cache + buf_read);
    auto list = buf.split("\r\n\r\n");
    if(!buf.endsWith("\r\n\r\n")){
        m_cache = list[list.size()-1];
        list.erase(list.end() - 1);
    }else{
        m_cache.clear();
    }
    auto self = shared_from_this();
    for(const auto msg : list){
        if(msg.isEmpty())
            continue;
        /*
        CTaskPool::Task task(msg,
                             [this, msg]()
        {
            CRequest req(msg);
            CResponse *resp = nullptr;
            Log::debug("do action "+req.action());
            CAction *p_action = CAction::create(req.action(), &req);
            if(nullptr == p_action){
                resp = new CResponse;
                resp->set_status_code(StatusCode::NO_ACTION);
                resp->set_desc("No such action");
            }else{
                p_action->run();
                resp = p_action->response();
            }
            resp->set_action(req.action());
            this->do_write(resp->to_json().toStdString());
        });
        TaskPool::add_task(task);
        */
        ThreadPool::do_work([this, msg, self]()
        {
            std::shared_ptr<CRequest> req = std::make_shared<CRequest>(msg);
            std::shared_ptr<CResponse> resp = nullptr;
            Log::debug("do action "+req->action());
            if(!Employee::check_access(req->action(), m_socket_id)){
                resp = std::make_shared<CResponse>();
                resp->set_status_code(StatusCode::INVAILD_ACCESS);
                resp->set_desc("invaild access");
            }else{
                CAction *p_action = nullptr;
                try{
                    p_action = CAction::create(req->action(), req);
                    if(nullptr == p_action){
                        resp = std::make_shared<CResponse>();
                        resp->set_status_code(StatusCode::NO_ACTION);
                        resp->set_desc("No such action");
                    }else{
                        p_action->set_socket_id(this->m_socket_id);
                        p_action->run();
                        resp = p_action->response();
                    }
                    delete p_action;
                }catch(std::bad_alloc e){
                    resp = std::make_shared<CResponse>();
                    resp->set_status_code(StatusCode::SYSTEM_BUSY);
                    resp->set_desc("system is busy now");
                }
            }
            resp->set_action(req->action());
            resp->set_started_by(req->started_by());
            this->do_write(resp->to_json().toStdString());
        });
    }
}

Tcp::socket &CTcpConnection::socket()
{
    return m_socket;
}
