/**
* @projectName   NEUPlateR_server
* @brief         the main thread to run server to accept connection
* @author        chenhanlin
* @date          2018-07-25
* @modify_author chenhanlin
* @modify_date   2018-07-25
*/
#ifndef ASYNC_SERVER_H
#define ASYNC_SERVER_H

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace Asio = boost::asio;
using Tcp = boost::asio::ip::tcp;

class CAsyncServer
{
public:
    CAsyncServer(Asio::io_context &io_contenxt,std::size_t port);
    void do_accept();
    void start();
    void stop();
private:
    void handle_error(const boost::system::error_code &ec);
    Asio::io_context &m_io_context;
    Tcp::acceptor m_acceptor;
};

#endif // ASYNC_SERVER_H
