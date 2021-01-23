#include "threadpool.h"

#include <QDebug>
#include <QDateTime>

namespace Base{

ThreadPool::ThreadPool(int threadNum):m_bStopped(false)
{
    int tnum = (threadNum <= 0 || threadNum > 10) ? 5 : threadNum;

    for(int i = 0; i < tnum; i++){
        m_threads.emplace_back([this](){
            while(true){

                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> ul(m_taskMutex);

                    condition.wait(ul,[&]{return m_bStopped || !m_taskQueue.empty();});

                    if(m_bStopped && m_taskQueue.empty())
                        return;

                    task = std::move(m_taskQueue.front());
                    m_taskQueue.pop();
                }

                task();
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lg(m_taskMutex);
        m_bStopped = true;
    }

    condition.notify_all();

    for(std::thread & tt:m_threads)
        tt.join();
}

} //namespace Base
