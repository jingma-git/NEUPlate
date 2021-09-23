#include "taskpool.h"
#include "asio_thread_pool.h"

CTaskPool::CTaskPool()
    :m_sep(0)
{

}

/**
* @functionName  add_task
* @brief         add a task into task pool
* @author        chenhanlin
* @date          2018-07-25
* @parameter     void
* @return        void
*/
void CTaskPool::add_task(const CTaskPool::Task &task)
{
    std::unique_lock<std::mutex> lock(locker);
    this->m_tasks.push(task);
    lock.unlock();
    m_sep.post();
}

/**
* @functionName  get_task
* @brief         get a task from pool and it's not thread safe
* @author        chenhanlin
* @date          2018-07-25
* @parameter     void
* @return        void
*/
CTaskPool::Task CTaskPool::get_task()
{
    m_sep.wait();
    Task task(m_tasks.front());
    m_tasks.pop();
    return task;
}

CTaskPool::Task::Task(const QString &req, const std::function<void()> &func)
    :req_str(req), callfunc(func) { }

//void TaskPool::add_task(const CTaskPool::Task &task)
//{
//    CAsioThreadPool::get_instance().enQueue(task);
//}
