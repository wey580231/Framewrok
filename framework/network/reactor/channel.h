/*!
 *  @brief     单个监视描述符信息
 *  @details   向eventloop注册自己，待有事件触发后进行数据回调
 *  @author    wey
 *  @version   1.0
 *  @date      2019.04.18
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
#include <QList>
#include "callbacks.h"

namespace Network{

class EventLoop;

#define S_POLLNONE 0x0000
#define S_POLLIN 0x0001
#define S_POLLOUT 0x0002

class Channel
{
public:
    Channel(EventLoop * loop,int sockId);
    ~Channel();

    int fd();
    void handleEvent();

    void updateStatus();
    void remove();

    void setReadCallback(ReadCallback callback){m_readCallback = callback;}
    void setWriteCallback(WriteCallback callback){m_writeCallback = callback;}

    /*!< 向poller注册channel需要关注的事件 */
    void enabelReading(){m_registEvents |= S_POLLIN;updateStatus();}
    void disableReading(){m_registEvents &= ~S_POLLIN;updateStatus();}
    bool isReading(){return m_registEvents & S_POLLIN;}

    void enableWriting(){m_registEvents |= S_POLLOUT;updateStatus();}
    void disableWriting(){m_registEvents &= ~S_POLLOUT;updateStatus();}
    bool isWriting(){return m_registEvents & S_POLLOUT;}

    void disableAll(){m_registEvents = S_POLLNONE;updateStatus();}

    /*!< 由poller设置channel产生的事件 */
    void setActiveEvents(int activeEvents){m_activeEvents = activeEvents;}
    bool readAble(){return m_activeEvents & S_POLLIN;}
    bool writeAble(){return m_activeEvents & S_POLLOUT;}

private:
    int m_i_sockFd;             /*!< 绑定的套接字描述符 */
    ReadCallback m_readCallback;
    WriteCallback m_writeCallback;
    EventLoop * m_loop;

    int m_registEvents;         /*!< 当前channel注册的事件，poller检测此变量状态 */
    int m_activeEvents;         /*!< 由select、poll等返回的激活事件 */
};

typedef QList<Channel *> ChannelList;

} //namespace Network

#endif // CHANNEL_H
