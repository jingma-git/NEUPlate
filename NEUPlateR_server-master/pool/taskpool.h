/**
* @projectName   NEUPlateR_server
* @brief         task queue, and it is not thread safe, only push function is thread safe
* @author        chenhanlin
* @date          2018-07-25
* @modify_author chenhanlin
* @modify_date   2018-07-25
*/
#ifndef TASKPOOL_H
#define TASKPOOL_H

#include <functional>
#include <string>
#include <boost/asio/ip/tcp.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <memory>
#include <mutex>
#include <queue>
#include "neu_head.h"

class CTaskPool
{
public:
    struct Task
    {
        QString req_str;
        std::function<void()> callfunc;
        std::function<void(const std::string &)> callback;
        Task(const QString&, const std::function<void()>&);
    };
public:
    CTaskPool();
    void add_task(const Task &task);
    Task get_task();
private:
    boost::interprocess::interprocess_semaphore m_sep;
    std::queue<Task> m_tasks;
    std::mutex locker;
};

//namespace TaskPool {
//void add_task(const CTaskPool::Task &task);
//}

#endif // TASKPOOL_H
