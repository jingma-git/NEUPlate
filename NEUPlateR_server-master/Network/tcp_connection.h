#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <QString>

namespace Asio = boost::asio;
using Tcp = boost::asio::ip::tcp;

class CTcpConnection : public std::enable_shared_from_this<CTcpConnection>
{
public:
    CTcpConnection(Asio::io_context &io_contenxt);
    ~CTcpConnection();
    void do_write(const std::string &data);
    void start();
    Tcp::socket& socket();

private:
    void do_read();
    void handle_error(boost::system::error_code ec);
    void handle_read(boost::system::error_code ec, std::size_t read_size);
private:
    static const int buf_size = 10 * 1024;
    Tcp::socket m_socket;
    QString m_address;
    QString m_cache;
    char buf_read[buf_size];
    QString m_socket_id;
};

#endif // TCP_CONNECTION_H
