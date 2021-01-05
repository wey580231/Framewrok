/*!
 *  @brief     网络事件循环
 *  @details   负责监视socket活动连接，可注册、移除监视的channel
 *  @author    wey
 *  @version   1.0
 *  @date      2019.04.18
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <thread>
#include <mutex>
#include <condition_variable>

#include <QThread>

#include "../network_global.h"

#include "channel.h"

namespace Network{

class Poller;

class NETWORKSHARED_EXPORT EventLoop : public QThread
{
public:
    explicit EventLoop();

    void updateChannel(Channel * ch);
    void removeChannel(Channel * ch);

    void startMe();
    void stopMe();

    bool isInLoopThread();
    void runInLoop(Functor fc);
    void queueInLoop(Functor fc);

protected:
    void run();

private:
    void processPendingCallbacks();

    /*!< 内部睡眠channel */
    void handleRead();
    void wakeup();

private:
    Poller * m_poller;
    ChannelList m_activeChannelList;
    int m_timeout;
    bool m_b_runningFlag;

    bool m_bChannelEventHandling;   /*!< true:正在处理socket事件 */
    bool m_bCallbackFuncHandling;   /*!< true:正在处理回调函数队列 */

    int m_defaultUdpSockId;         /*! udp唤醒句柄 */
    int m_clientUdpSockId;
    ushort m_defaultUdpPort;        /*!< udp监听端口，同一个进程内唯一。暂不支持一个机器内启动多个进程 */
    Channel * m_defaultChannel;     /*! 默认eventloop拥有一个channel用于随时唤醒线程阻塞，避免空转 */

    std::mutex m_peddingMutex;
    std::vector<Functor> m_peddingFunctors;        /*!< 待统一处理的回调函数 */

    std::thread::id m_currentId;    /*!< 当前线程Id，区分调用者线程是否和当前线程一致 */
};

} //namespace Network

#endif // EVENTLOOP_H
