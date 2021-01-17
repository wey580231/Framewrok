#include "rthread.h"

#include <iostream>

namespace Network{

struct RThreadData
{
    RThreadData(const ThreadFunc & func,const string & name):m_func(func),m_threadName(name){
    }

    ThreadFunc m_func;
    string m_threadName;

    void runInThread(){
        try{
            m_func();
        }catch(...){
            cout<<"unprocess exception~"<<endl;
        }
    }
};

void startInThread(void * obj)
{
    RThreadData * data = static_cast<RThreadData *>(obj);
    data->runInThread();
    delete data;
}

RThread::RThread(const ThreadFunc func, const string &tname):m_func(func),m_threadName(tname),m_started(false)
{

}

RThread::~RThread()
{
    join();
    cout<<"destructor rthread~~"<<endl;
}

void RThread::start()
{
    RThreadData * t_data = new RThreadData(m_func,m_threadName);
    m_thread = std::move(std::thread(startInThread,t_data));
    m_started = true;
}

void RThread::join()
{
    if(m_thread.joinable())
        m_thread.join();
}

bool RThread::joinable()
{
    return m_thread.joinable();
}

} //namespace Network{
