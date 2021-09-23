#include "async_server.h"
#include <memory>
#include <boost/bind.hpp>
#include "tcp_connection.h"
#include "pool/asio_thread_pool.h"
#include "neu_head.h"
#include "log/log_handler.h"

CAsyncServer::CAsyncServer(boost::asio::io_context &io_contenxt, std::size_t port)
    :m_io_context(io_contenxt),
      m_acceptor(m_io_context, Tcp::endpoint(Tcp::v4(), port))
{
    Log::debug(QString("start listen at 0.0.0.0:%1").arg(port));
    this->do_accept();
}

/**
* @functionName  do_accpet
* @brief         accept connection from nerwork
* @author        chenhanlin
* @date          2018-07-25
* @parameter     void
* @return        void
*/
void CAsyncServer::do_accept()
{
    auto conn = std::make_shared<CTcpConnection>(m_io_context);
    m_acceptor.async_accept(conn->socket(),
                            [this, conn](boost::system::error_code ec)
    {
        if(!ec){
            conn->start();
            this->do_accept();
        }else{
            handle_error(ec);
        }
    });
}

/**
* @functionName  start
* @brief         start to runing server
* @author        chenhanlin
* @date          2018-07-25
* @parameter     void
* @return        void
*/
void CAsyncServer::start()
{
    m_io_context.run();
}

/**
* @functionName  stop
* @brief         stop the server to run
* @author        chenhanlin
* @date          2018-07-26
* @parameter     void
* @return        void
*/
void CAsyncServer::stop()
{
    this->m_io_context.stop();
}

/**
* @functionName  handle_error
* @brief         print error message
* @author        chenhanlin
* @date          2018-08-01
* @parameter     void
* @return        void
*/
void CAsyncServer::handle_error(const boost::system::error_code &ec)
{
//    Log::debug(ec.message().c_str());
}
