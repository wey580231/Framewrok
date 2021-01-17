#include "globalmailbox.h"

#include <QDebug>

#include <base/util/rutil.h>
#include <base/util/rsingleton.h>
#include "../../global.h"
#include "../interface/rcomponent.h"
#include "../plugin/pluginmanager.h"

namespace Core{

GlobalMailBox* GlobalMailBox::m_instance = nullptr;

GlobalMailBox::GlobalMailBox()
{

}

GlobalMailBox::~GlobalMailBox()
{

}

GlobalMailBox *GlobalMailBox::instance()
{
    if(m_instance == nullptr)
        m_instance = new GlobalMailBox;
    return m_instance;
}

void GlobalMailBox::updateTopicMap(QMap<QString, QStringList> &map)
{
    m_topicPluginMap = map;
}

/*!
* @brief 更新订阅系统主题信息集合
* @param[in] map 订阅系统信息集合
*/
void GlobalMailBox::updateSystemTopicMap(QMap<QString, QStringList> &map)
{
    m_systemPluginMap = map;
}

QStringList GlobalMailBox::querySystemMsgSubscriblePluginId(QString systemMsgId)
{
    return m_systemPluginMap.value(systemMsgId);
}

/*!
 * @brief 发布插件间消息【异步调用】
 * @attention 返回false时，需手动删除消息
 * @param[in] topicId 主题号
 * @param[in] mailMessage 发布信息
 * @param[in] pty 消息优先级
 * @return 发送成功返回true，若没有订阅者返回false
 */
bool GlobalMailBox::postPluginMessage(QString topicId, PluginMessage *mailMessage, Base::ElementPriority pty )
{
    if(m_topicPluginMap.contains(topicId) && mailMessage != nullptr)
    {
        QStringList subjectList = m_topicPluginMap.value(topicId);
        if(subjectList.size() > 0){
            mailMessage->refCount = subjectList.size();
            mailMessage->type = M_Plugin;
            std::for_each(subjectList.begin(),subjectList.end(),[&](QString subjectId){
                mailMessage->targetId = subjectId;
                mailMessage->message.topicId = topicId;
                RComponent * taregetPlugin = PluginManager::instance()->getActivePlugin(subjectId);
                if(taregetPlugin){
                    taregetPlugin->recvMail(mailMessage,pty);
                }
            });
            return true;
        }
    }
    return false;
}

/*!
 * @brief 发送从系统到插件信息【异步调用】
 * @param[in] mailMessage 发布信息
 * @param[in] pty 消息优先级
 * @return 发送成功返回true，若没有订阅者返回false
 */
bool GlobalMailBox::postSystemMessage(Datastruct::PluginMessage *mailMessage,Base::ElementPriority pty)
{
    if(mailMessage){
        mailMessage->type = M_System;

        if(mailMessage->message.m_broadCast){
            PluginManager::ActiveComponentMap t_map = PluginManager::instance()->getAllActivePlugins();
            mailMessage->refCount = t_map.size();
            PluginManager::ActiveComponentMap::iterator iter = t_map.begin();
            while(iter != t_map.end()){
                RComponent * taregetPlugin = iter.value().pComp;
                mailMessage->targetId = taregetPlugin->id().toString();
                taregetPlugin->recvMail(mailMessage,pty);
                iter++;
            }
        }else{
            //TODO 20190929 需插件主动向框架注册接收的信息，否则框架无法获取插件的id
            QStringList targetlist = mailMessage->targetId.split(";");
            mailMessage->refCount = targetlist.size();
            std::for_each(targetlist.begin(),targetlist.end(),[&](QString targetId){
                mailMessage->targetId = targetId;
                RComponent * taregetPlugin = PluginManager::instance()->getActivePlugin(targetId);
                if(taregetPlugin){
                    taregetPlugin->recvMail(mailMessage,pty);
                }
            });
        }
    }
    return false;
}

/*!
 * @brief 从插件向系统发送消息【同步调用】
 * @param[in] mailMessage 发布信息
 * @return 发送成功返回true
 */
void GlobalMailBox::postToSystem(PluginMessage *mailMessage)
{
    if(mailMessage != nullptr){
        emit sendToSystem(mailMessage);
    }
}

/*!
* @brief 向状态显示状态栏发送系统运行信息
* @param[in] level 信息等级
* @param[in] content 消息内容
* @param[in] srcComponent 消息发出源插件/模块
*/
void GlobalMailBox::postStatusInfo(Datastruct::StatusLevel level, QString content, QString srcComponent)
{
    Datastruct::SystemStatusInfo info;
    info.m_level = level;
    info.m_content = content;
    info.m_src = srcComponent;
    info.m_timestamp = Base::RUtil::getTimeStamp("hh:mm:ss");

    RGlobal::G_StatusInfoQueue.put(std::move(info));
}


} //namespace Core
