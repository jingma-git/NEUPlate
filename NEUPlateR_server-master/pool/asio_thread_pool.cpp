#include "asio_thread_pool.h"
#include <iostream>
#include <boost/bind.hpp>
#include <Route/action.h>

CAsioThreadPool::CAsioThreadPool(std::size_t size)
    :m_work(new Work(Asio::make_work_guard(m_io_context))),
      m_keep_run(true)
{
    for(std::size_t i = 0; i < size; i++){
        m_threads.emplace_back([this](){
            m_io_context.run();
        });
    }
}

/**
* @functionName  release
* @brief         close the pool, release all thread
* @author        chenhanlin
* @date          2018-07-25
* @parameter     void
* @return        void
*/
void CAsioThreadPool::release()
{
    m_keep_run = false;
    m_work.reset();
    for(auto &t : m_threads){
        t.join();
    }
}

/*
void CAsioThreadPool::start()
{
    while(m_keep_run){
        CTaskPool::Task task(m_task_queue.get_task());
        m_io_context.post(task.callfunc);
    }
}
*/

/**
* @functionName  get_instance
* @brief         singleton partten
* @author        chenhanlin
* @date          2018-07-25
* @parameter     void
* @return        void
*/
CAsioThreadPool &CAsioThreadPool::get_instance()
{
    static CAsioThreadPool pool(700);
    return pool;
}

/**
* @functionName  enQueue
* @brief         add task into task queue
* @author        chenhanlin
* @date          2018-07-25
* @parameter     void
* @return        void
*/
//void CAsioThreadPool::enQueue(const CTaskPool::Task &task)
//{
//    this->m_task_queue.add_task(task);
//}

/**
* @functionName  add_task
* @brief         give task to thread pool to execute
* @author        chenhanlin
* @date          2018-07-26
* @parameter     const std::function<void()> &func function need to execute
* @return        void
*/
void CAsioThreadPool::add_task(const std::function<void ()> &func)
{
    this->m_io_context.post(func);
}

/**
* @functionName  do_work
* @brief         the interface of add_task
* @author        chenhanlin
* @date          2018-07-26
* @parameter     void
* @return        void
*/
void ThreadPool::do_work(const std::function<void()> &func)
{
    CAsioThreadPool::get_instance().add_task(func);
}
