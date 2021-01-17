/*!
 *  @brief     全局信息邮箱中转站
 *  @details   1.负责接收所有系统、插件产生的信息；
 *             2.负责将信息投递至每个插件邮箱内;
 *  @author    wey
 *  @version   1.0
 *  @date      2019.01.18
 *  @warning   【投递后的信息必须手动调用 的删除，否则会出现内存泄漏问题】
 *  @copyright NanJing RenGu.
 *  @note
 *      20190912:wey:增加对全局状态信息投递
 */
#ifndef GLOBALMAILBOX_H
#define GLOBALMAILBOX_H

#include <QObject>

#include <base/util/rblockingqueue.h>
#include "../../protocol/datastruct.h"

namespace Core{

class CORESHARED_EXPORT GlobalMailBox : public QObject
{
    Q_OBJECT
public:
    static GlobalMailBox * instance();

    void updateTopicMap(QMap<QString,QStringList> & map);

    void updateSystemTopicMap(QMap<QString,QStringList> & map);
    QStringList querySystemMsgSubscriblePluginId(QString systemMsgId);

    bool postPluginMessage(QString topicId,Datastruct::PluginMessage * mailMessage,Base::ElementPriority pty = Base::NormalPriority);

    bool postSystemMessage(Datastruct::PluginMessage * mailMessage,Base::ElementPriority pty = Base::NormalPriority);
    void postToSystem(Datastruct::PluginMessage * mailMessage);

    void postStatusInfo(Datastruct::StatusLevel level,QString content,QString srcComponent);

signals:
    void sendToSystem(Datastruct::PluginMessage *);

private:
    explicit GlobalMailBox();
    GlobalMailBox(const GlobalMailBox & box) = delete;
    GlobalMailBox& operator=(const GlobalMailBox & box) = delete;
    ~GlobalMailBox();

private:
    QMap<QString,QStringList> m_topicPluginMap;     /*!< key:主题号,value:订阅此主题的插件ID集合 */
    QMap<QString,QStringList> m_systemPluginMap;    /*!< 系统向插件发送的信息集合：key:主题号,value:订阅此主题的插件ID集合 */
    static GlobalMailBox * m_instance;

};

} //namespace Core

#endif // GLOBALMAILBOX_H
