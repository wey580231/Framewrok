/*!
 *  @brief     通用组件窗口类
 *  @details   1.定义了组件对组件管理者暴露接口 @n
 *             2.绑定MyDockWidget作为插件容器,管理其生命周期，由其承载子窗口； @n
 *             3.系统-插件、插件-插件之间的信息传输取消原来的直接回调形式，修改为通过消息简单消息机制。当系统需要将调用插件时，将调用信息封装成消息PluginMessage，投递至全局
 *               消息处理邮箱，由全局消息根据消息的目的信息，投递至对方插件的邮箱内。由目的插件自身通过消息处理机制逐个处理，通过这种方式可以避免主线程因处理数据业务时间长，造成 @n
 *               界面卡顿情况。
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 *      20181113:wey:将父类从QDockWidget改为QObject，将继承关系改为依赖
 *      20190110:wey:添加对原始数据处理
 *      20190118:wey:为每个插件增加一个信箱接收
 */
#ifndef RCOMPONENT_H
#define RCOMPONENT_H

#include <base/actionmanager/id.h>
#include <base/messagetype.h>
#include <base/util/rblockingqueue.h>
#include <core/pluginmanager/mailbox/observer.h>
#include <core/protocol/waitimplemneted.h>
#ifdef OFFICAL_DOCK
#include <core/officalwidget/rdockwidget.h>
#else
#include <core/selfwidget/mydockwidget.h>
#endif
#include <core/core_global.h>

#include "../mailbox/pluginmailbox.h"

namespace Core{

class MyDockWidget;

class CORESHARED_EXPORT RComponent : public QObject
{
    Q_OBJECT
public:
    RComponent(const char * id,QObject * parent  = 0);
    RComponent(const RComponent &) = delete;
    RComponent& operator= (const RComponent &) = delete;
    virtual ~RComponent();

    /*!< 插件基本信息 */
	Base::Id id();
    QString getPluginId();

    QString name();
    void setName(QString newName){m_name = newName;}

#ifdef OFFICAL_DOCK
    void setDockContainer(RDockWidget * container){this->dockContainer = container;}
    RDockWidget * getDockContainer(){return this->dockContainer;}
#else
    void setDockContainer(MyDockWidget * container){this->dockContainer = container;}
    MyDockWidget * getDockContainer(){return this->dockContainer;}
#endif

    void setModuleInfo(Datastruct::ModuleInfo & info){this->localModuleInfo = info;}
    Datastruct::ModuleInfo getModuleinfo(){return this->localModuleInfo;}

    /*!< 插件生命周期 */
    void initializeMailBox();
    virtual QWidget * initialize(QWidget * parent) = 0;
    virtual void release() = 0;
    virtual QString pluginName();
    virtual RComponent * clone(){return NULL;}
    virtual void closing(){}

    /*!< 接收新消息 */
    void recvMail(Datastruct::PluginMessage * mail,Base::ElementPriority pty = Base::NormalPriority);

    /*!< 处理系统信息/网络广播 */
    virtual void onMessage(PluginMessageList mails) = 0;

    /*!< 插件订阅外部主题集合 */
    virtual QStringList subscribeTopic();

    /*!< 插件向外部发布主题集合 */
    virtual QStringList publishTopic();

    /*!< 插件订阅框架主题集合 */
    virtual QStringList subscribeSystemMsg();

    /*!< TCP、UDP原始数据回调，插件自己解析 */
    virtual void onNetWork(QByteArray & originalData,QString remoteIp){}

    /*!< 获取当前插件订阅的协议信息 */
    ProtocolDataStaticInfoList getStaticInfoList(){return m_protocolStatics;}

private slots:
    void newMailComing(Datastruct::PluginMessageList mails);

protected:
    virtual void retranslateUi(){}

protected:
	Base::Id m_id;                      /*!< 插件唯一Id，用于身份访问。插件创建时自动产生 */
    QString pluginId;             /*!< 如0x0001,用于在plugin.xml中引用 */
    QString m_name;               /*!< dockwidget titlebar */
    static int compIndex;         /*!< 组件索引 */
#ifdef OFFICAL_DOCK
    RDockWidget * dockContainer;               /*!<  插件绑定的容器 */
#else
    MyDockWidget * dockContainer;
#endif
    Datastruct::ModuleInfo localModuleInfo;     /*!< plugin.xml文件中模块配置信息 */
    ProtocolDataStaticInfoList m_protocolStatics;   /*!< 当前插件需使用的协议集合 */

    PluginMailBox * mailBox;       /*!< 数据信息邮箱 */
    bool mailProcessOver;          /*!< 数据处理状态标识,为false时,邮箱线程收到信息不会再次发出信息号 */

    friend class PluginMailBox;
};

} //namespace Base

#define RComponentInterface_iid "com.rengu.viewframe"
Q_DECLARE_INTERFACE(Core::RComponent, RComponentInterface_iid)

#endif // RCOMPONENT_H
