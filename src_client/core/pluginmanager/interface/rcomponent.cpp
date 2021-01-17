#include "rcomponent.h"

#include <QDebug>
#include <base/util/rsingleton.h>
#include "core/constants.h"

namespace Core{

int RComponent::compIndex = 0;

RComponent::RComponent(const char *id, QObject *parent):
    QObject(parent),m_id(QString("%1_%2").arg(id).arg(compIndex++).toLocal8Bit().data()),dockContainer(NULL)
    ,mailBox(nullptr),mailProcessOver(true)
{
    setObjectName(m_id.toString());
    retranslateUi();
}

RComponent::~RComponent()
{
    closing();
    if(mailBox)
        mailBox->stopMe();
}

/*!
 * @brief 插件运行时唯一标识
 * @attention 标识符会因为所加载插件的数量、顺序不同而不唯一
 * @details 插件在实例化时根据当前插件的objname和实例化的索引，动态创建一个唯一的标识
 * @return 插件运行期间唯一标识
 */
Base::Id RComponent::id()
{
    return m_id;
}

/*!
 * @brief 插件模板标识
 * @details 每个插件的序号由插件自行指定，不可重复。此值与plugins.xml/plugins节点中id保持一致
 * @return 模板标识号
 */
QString RComponent::getPluginId()
{
    return pluginId;
}

/*!
 * @brief 运行时窗口时标签栏名称
 * @details dock和tab窗口状态下显示的标题，此值与plugins.xml/modules节点中name保持一致
 */
QString RComponent::name()
{
    return m_name;
}

/*!
 * @brief 插件默认名称
 */
QString RComponent::pluginName()
{
    return m_name;
}

/*!
 * @brief 新的信息到达插件
 * @details 将信息加入至插件邮箱，等待处理
 * @param[in]   mail 新信息
 */
void RComponent::recvMail(PluginMessage *mail, Base::ElementPriority pty)
{
    if(mailBox)
        mailBox->recvMail(mail,pty);
}

/*!
 * @brief 订阅其它其它插件的主题
 * @details 1.主题以字符串表示，两个字符串相等，即表示为同一主题;
 *          2.系统收集各个插件的发布-订阅的关系，负责根据主题查找对应的订阅者，将同一消息投递至对应插件的接收箱内
 */
QStringList RComponent::subscribeTopic()
{
    return QStringList();
}

/*!
 * @brief 发布主题
 * @param[in] 1.代表此插件对外提供的信息类型,接收方在可订阅插件发布的主题，待此插件发布对应的主题消息时，接收方可接收此主题消息
 */
QStringList RComponent::publishTopic()
{
    return QStringList();
}

/*!
 * @brief 订阅系统信息
 * @details 1.主题以字符串表示，两个字符串相等，即表示为同一主题;
 *          2.系统收集各个插件的订阅的关系，负责根据主题查找对应的订阅者，将同一消息投递至对应插件的接收箱内
 */
QStringList RComponent::subscribeSystemMsg()
{
    return QStringList();
}

void RComponent::newMailComing(Datastruct::PluginMessageList mails)
{

    this->onMessage(mails);
    mailProcessOver = true;
    if(mailBox)
        mailBox->poll();
}

/*!
* @brief 初始化接收信箱
* @attention 此方法只调用一次即可
* @details 为了避免在dll加载时候即初始化线程接收信箱，将初始化延迟到插件初始化时调用
*/
void RComponent::initializeMailBox()
{
	if(mailBox == nullptr){
		mailBox = new PluginMailBox(this);
    	connect(mailBox,SIGNAL(dataReady(Datastruct::PluginMessageList)),this,SLOT(newMailComing(Datastruct::PluginMessageList)));
   	 	mailBox->startMe();
	}
}

} //namespace Base
