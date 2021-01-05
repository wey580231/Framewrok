#include "eventloop.h"

#include <QDebug>

#include "socketsops.h"
#include "poller/selectpoller.h"

namespace Network{

std::mutex G_PortMutex;

int getListenPort()
{
    //TODO 20191107 待修改成内存共享模式
    static int t_basePort = 19727;
    std::lock_guard<std::mutex> lg(G_PortMutex);
    return ++t_basePort;
}

int creteUdpSocket(ushort listenPort)
{
    int sock = Sock::createUdpSock();
    if(sock > 0 && Sock::rbind(sock,"127.0.0.1",listenPort) == 0){
        return sock;
    }
    return -1;
}

EventLoop::EventLoop():m_b_runningFlag(true),m_poller(new SelectPoller()),m_timeout(3),
    m_bChannelEventHandling(false),m_bCallbackFuncHandling(false)
{
#ifdef Q_OS_WIN
    static bool isInit = false;
    if(!isInit)
    {
        isInit = true;
        WSADATA ws;
        if(WSAStartup(MAKEWORD(2,2),&ws) != 0)
        {
            qDebug()<<"Init windows socket failed!";
        }
        qDebug()<<"init windows socket success!";
    }
#endif

    m_clientUdpSockId = Sock::createUdpSock();
    m_defaultUdpPort = getListenPort();
    m_defaultUdpSockId = creteUdpSocket(m_defaultUdpPort);

    m_defaultChannel = new Channel(this,m_defaultUdpSockId);
    m_defaultChannel->setReadCallback(std::bind(&EventLoop::handleRead,this));
    m_defaultChannel->enabelReading();
}

void EventLoop::updateChannel(Channel *ch)
{
    m_poller->updateChannel(ch);
}

void EventLoop::removeChannel(Channel *ch)
{
    m_poller->removeChannel(ch);
}

void EventLoop::startMe()
{
    start();
}

void EventLoop::stopMe()
{
    //TODO 20191224 待释放线程及关闭连接
}

bool EventLoop::isInLoopThread()
{
    return m_currentId == std::this_thread::get_id();
}

/*!
 * @brief 在I/O线程中运行，否则加入回调队列
 * @attention 先检测当前函数回调的线程与当前线程是否一致，若一致则可直接调用，否则将回调加入待处理队列。避免跨线程调用
 * @param[in] fc 待处理回调函数
 */
void EventLoop::runInLoop(Functor fc)
{
    if(isInLoopThread()){
        fc();
    }else{
        queueInLoop(fc);
    }
}

/*!
 * @brief 等待处理各个监测socket上的事件
 */
void EventLoop::run()
{
    m_currentId = std::this_thread::get_id();

    while(m_b_runningFlag){

        m_activeChannelList.clear();
        m_poller->poll(m_timeout,m_activeChannelList);

        m_bChannelEventHandling = true;
        std::for_each(m_activeChannelList.begin(),m_activeChannelList.end(),[&](Channel * channel){
            channel->handleEvent();
        });

        m_bChannelEventHandling = false;

        processPendingCallbacks();
    }
}

/*!
 * @brief 处理在当前线程中等待的回调函数
 * @details 1.处理非当前线程中产生的调用操作，避免多线程同时对socket进行write操作；
 *          2.处理在事件处理中产生的回调事件，
 */
void EventLoop::processPendingCallbacks()
{
    m_bCallbackFuncHandling = true;

    std::vector<Functor> t_funtorList;
    {
        std::lock_guard<std::mutex> lg(m_peddingMutex);
        m_peddingFunctors.swap(t_funtorList);
    }

    for(const Functor & func:t_funtorList){
        func();
    }

    m_bCallbackFuncHandling = false;
}

void EventLoop::queueInLoop(Functor fc)
{
    {
        std::lock_guard<std::mutex> lg(m_peddingMutex);
        m_peddingFunctors.push_back(std::move(fc));
    }

    if(!isInLoopThread() || m_bCallbackFuncHandling){
        wakeup();
    }
}

void EventLoop::handleRead()
{
    char buff[1024];
    int rlen = Sock::recvFrom(m_defaultChannel->fd(),buff,1024);
    if(rlen == 0){
        qDebug()<<"wake up error~~";
    }
}

/*!
 * @brief 往默认socket上写一个字节数据，唤醒poller
 */
void EventLoop::wakeup()
{
    char buff = 'a';
    int wlen = Sock::sendTo(m_clientUdpSockId,&buff,1,"127.0.0.1",m_defaultUdpPort);
    if(wlen != 1){
        qDebug()<<"wake up error====";
    }
}

} //namespace Network
