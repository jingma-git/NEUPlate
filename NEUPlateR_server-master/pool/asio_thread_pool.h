/**
* @projectName   NEUPlateR_server
* @brief         ThreadPool, it is the sync layer, to execute logic function
* @author        chenhanlin
* @date          2018-07-25
*/
#ifndef ASIO_THREAD_POOL_H
#define ASIO_THREAD_POOL_H

#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <functional>
#include "taskpool.h"

namespace Asio = boost::asio;

class CAsioThreadPool
{
public:
    CAsioThreadPool(std::size_t size=std::thread::hardware_concurrency());
    void release();
//    void start();
    static CAsioThreadPool& get_instance();
//    void enQueue(const CTaskPool::Task &task);
    void add_task(const std::function<void()> &func);
private:
    using Work = Asio::executor_work_guard<Asio::io_context::executor_type>;
    Asio::io_context m_io_context;
    std::unique_ptr<Work> m_work;
    std::vector<std::thread> m_threads;
//    CTaskPool m_task_queue;
    bool m_keep_run;
};

namespace ThreadPool {
void do_work(const std::function<void()> &func);
}

#endif // ASIO_THREAD_POOL_H
