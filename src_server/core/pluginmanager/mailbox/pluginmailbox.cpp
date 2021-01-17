#include "pluginmailbox.h"

#include "../interface/rcomponent.h"

#include <QDebug>

namespace Core{

PluginMailBox::PluginMailBox(RComponent * comp, QObject *parent):AbstractMailBox(parent),pollSize(1)
{
    component = comp;
    pluginId = comp->getPluginId();
}

PluginMailBox::~PluginMailBox()
{

}

/*!
 * @brief 设置单次从信箱中获取信息的大小
 * @attention 最大一次允许设置1000个信息空间
 * @details 此值由插件自己设置，需根据业务场景来设置;
 * @param[in] maxSize 一次最大获取值的数量
 */
void PluginMailBox::setPollSize(int maxSize)
{
    if(maxSize <= 0)
        return;

    pollSize = maxSize > 1000 ? 1000 : maxSize;
}

/*!
 *  @brief 获取邮件信息
 */
int PluginMailBox::getMailSize()
{
    return m_recvMailBox.size();
}

/*!
 * @brief 拉取新信息
 * @details 由插件主动通知获取下一批信息
 */
void PluginMailBox::poll()
{
    if(component->mailProcessOver){
        m_recvMailBox.wakeUpConsumer();
    }
}

void PluginMailBox::recvMail(Datastruct::PluginMessage *mail, Base::ElementPriority pty)
{
    m_recvMailBox.put(mail,pty);
    poll();
}

void PluginMailBox::run()
{
    while(runningFlag){
        while(m_recvMailBox.isEmpty() || !component->mailProcessOver){
            m_recvMailBox.wait();
        }

        if(!m_recvMailBox.isEmpty()){
            component->mailProcessOver = false;
            Datastruct::PluginMessageList list;

            {
                std::list<Datastruct::PluginMessage *> t_result = m_recvMailBox.takeMost(pollSize);
                while(!t_result.empty()){
                    list.push_back(t_result.front());
                    t_result.pop_front();
                }
            }

            emit dataReady(list);
        }
    }
}

} //namespace Core
