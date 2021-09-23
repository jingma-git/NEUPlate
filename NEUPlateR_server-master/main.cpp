#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "log/log_handler.h"
#include "Network/async_server.h"
#include "pool/asio_thread_pool.h"
#include "pool/connection_pool.h"
#include "Business/Employee/employee_controller.h"
#include "Business/Deduction/deduction_controller.h"
#include "Business/RecogRecord/recog_record_controller.h"
#include "Business/Toll/toll_controller.h"

void rename_log_file(boost::asio::deadline_timer *timer){
    LogHandler::get_instance().open_and_backup_file();
    timer->async_wait(boost::bind(rename_log_file, timer));
}

void flush_log_file(boost::asio::deadline_timer *timer){
    LogHandler::get_instance().flush_log_file();
    timer->async_wait(boost::bind(flush_log_file, timer));
}

void free_db_connection(boost::asio::deadline_timer *timer){
    ConnectionPool::get_instance().release_free_connection();
    timer->async_wait(boost::bind(free_db_connection, timer));
}

void handle_signale(boost::system::error_code ec, int signal, CAsyncServer *server){
    server->stop();
}

int main(int argc, char *argv[])
{
    LogHandler::get_instance().install_message_handler();
    Employee::init_access();
    ConnectionPool::get_instance();
    CAsioThreadPool::get_instance();
    boost::asio::io_context io_context;

    boost::asio::deadline_timer rename_file_timer(io_context, boost::posix_time::seconds(60 * 60));
    boost::asio::deadline_timer flush_file_timer(io_context, boost::posix_time::seconds(60));
    boost::asio::deadline_timer free_db_timer(io_context, boost::posix_time::seconds(60 * 20));
    boost::asio::signal_set sig(io_context, SIGINT, SIGTERM);
    CAsyncServer server(io_context, 1234);

    rename_file_timer.async_wait(boost::bind(rename_log_file, &rename_file_timer));
    flush_file_timer.async_wait(boost::bind(flush_log_file, &flush_file_timer));
    free_db_timer.async_wait(boost::bind(free_db_connection, &free_db_timer));
    sig.async_wait(boost::bind(handle_signale, _1, _2, &server));

    server.start();
    Log::info("stop system successful");
    CAsioThreadPool::get_instance().release();
}
