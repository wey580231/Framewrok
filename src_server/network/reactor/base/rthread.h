/*!
 *  @brief     自定义线程
 *  @details   对std::thread进行封装
 *  @author    wey
 *  @version   1.0
 *  @date      2019.11.15
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RTHREAD_H
#define RTHREAD_H

#include <thread>
#include <string>

using namespace std;

namespace Network{

typedef std::function<void()> ThreadFunc;

class RThread
{
public:
    RThread(const ThreadFunc func,const string & tname);
    ~RThread();

    void start();
    void join();

    bool isStarted() const {return m_started;}
    bool joinable();

private:
    bool m_started;

    std::thread m_thread;

    ThreadFunc m_func;
    string m_threadName;
};

} //namespace Network

#endif // RTHREAD_H
