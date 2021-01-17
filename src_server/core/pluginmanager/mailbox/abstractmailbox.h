/*!
 *  @brief     通信数据邮箱
 *  @details   本系统将网络数据、插件间信息、系统信息转换成统一数据格式PluginMessage(邮件);
 *             1.系统拥有全局邮箱用于系统<->插件、插件<->插件间数据通信；
 *             2.每个插件拥有数据自己的接收邮箱；
 *             在传递数据时，将产生的邮件投递至接全局邮箱，由全局邮箱根据目的地投递至接收方的邮箱内
 *  @author    wey
 *  @version   1.0
 *  @date      2019.01.18
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef ABSTRACTMAILBOX_H
#define ABSTRACTMAILBOX_H

#include <base/util/rblockingqueue.h>
#include "core/network/rtask.h"
#include "core/protocol/datastruct.h"

namespace Core{

class AbstractMailBox : public RTask
{
    Q_OBJECT
public:
    explicit AbstractMailBox(QObject * parent = 0);
    virtual ~AbstractMailBox();

    virtual void recvMail(Datastruct::PluginMessage *mail,Base::ElementPriority pty = Base::NormalPriority) = 0;

    void startMe();
    void stopMe();

protected:
    typedef QQueue<Datastruct::PluginMessage * > MailBox;

    Base::RBlockingQueque<Datastruct::PluginMessage *> m_recvMailBox;

};

} //namespace Core

#endif // ABSTRACTMAILBOX_H
