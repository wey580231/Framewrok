#include "statusbar.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#pragma comment(lib,"user32.lib")
#endif

#include <QHBoxLayout>
#include <QDebug>
#include <QMenu>
#include <QCursor>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QTimer>

#include <base/util/rutil.h>
#include <base/actionmanager/actioncontainer.h>
#include <base/actionmanager/actionmanager.h>
#include <base/actionmanager/action.h>
#include <base/util/rlog.h>
#include "../file/appconfig.h"
#include "../officalwidget/statusbaritemparsemethod.h"
#include "../file/programfilepath.h"
#include "../global.h"
#include "../file/globalconfigfile.h"

namespace Core{

class StatusBarPrivate{
    Q_DECLARE_PUBLIC(StatusBar)
public:
    StatusBarPrivate(StatusBar * q):q_ptr(q),m_rightContextMenu(nullptr),m_delaySaveTimer(nullptr){
        m_rightContextMenu = new Base::MenuActionContainer(Base::Id("StatusBarContextMenu"));
        QMenu * menu = new QMenu();
        m_rightContextMenu->setMenu(menu);
        init();
    }
    void init();

    StatusBar * q_ptr;

    QWidget * mainWidget;
    QHBoxLayout * m_itemLayout;
	Base::MenuActionContainer * m_rightContextMenu;
    QList<Base::StatusBarContainer * > m_groups;
    QTimer * m_delaySaveTimer;          /*!< 延时保存配置文件定时器 */
};

void StatusBarPrivate::init()
{
    mainWidget = new QWidget(q_ptr);
#ifndef OFFICAL_DOCK
    mainWidget->setObjectName("mainWidget");
#endif

    QWidget * widget = new QWidget();
    m_itemLayout = new QHBoxLayout();
    m_itemLayout->setContentsMargins(1,1,1,1);
    widget->setLayout(m_itemLayout);

    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(widget);
    mainWidget->setLayout(mainLayout);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(mainWidget);
    q_ptr->setLayout(layout);
}

StatusBar::StatusBar(QWidget * parent):Widget(parent),d_ptr(new StatusBarPrivate(this))
{
    m_statusBar = this;

    setFocusPolicy(Qt::ClickFocus);
    addWidgetFeatures(WidgetDialog);
    enableDialogWindowModule(testFeatures(WidgetDialog));
    setAttribute(Qt::WA_TranslucentBackground);

    setFixedHeight(STATUSBAR_FIX_HEIGHT);
    show();
}

StatusBar::~StatusBar()
{
    Q_D(StatusBar);
    std::for_each(d->m_groups.begin(),d->m_groups.end(),[&](Base::StatusBarContainer * cont){
        delete cont;
    });
}

StatusBar * StatusBar::m_statusBar = nullptr;

StatusBar*  StatusBar::instance()
{
    return m_statusBar;
}

/*!
* @brief 固定当前位置在底部
* @details 1.任务栏固定显示在最底部（需考虑全屏或任务栏隐藏以及任务栏在左、上、右、下的情形）；
*          2.若没有全屏，并且任务栏可见，则显示在任务栏之上；若任务栏自动隐藏，则自动显示在屏幕底部
*/
void StatusBar::fixedPosBottom()
{
    int t_iTaskHeight = 0;      //任务栏高度
    QSize t_screenSize = Base::RUtil::screenSize();
#ifdef Q_OS_WIN
    if(!RGlobal::G_GlobalConfigFile->systemConfigInfo.fullscreen){
        RECT t_rect;
        ::GetWindowRect(::FindWindowW(TEXT("Shell_TrayWnd"),NULL),&t_rect);
        if(t_rect.bottom > t_screenSize.height() / 2){
            if(t_rect.bottom <= t_screenSize.height()){
                t_iTaskHeight = t_rect.bottom - t_rect.top;
            }
        }
    }
#endif
    setGeometry(0,t_screenSize.height() - t_iTaskHeight - height(),t_screenSize.width(),height());
}

/*!
* @brief 加载本地的配置信息，更新当前控件显示
*/
void StatusBar::loadLocalSetting()
{
    Q_D(StatusBar);
    ProgramFilePath filepath;
    Base::RXmlFile tfile(filepath.statusbarConfigPath);
    StatusBarItemParseMethod method;
    tfile.setParseMethod(&method,false);
    if(!tfile.startParse()){
        RLOG_ERROR("save statusbar config file error!");
        return;
    }

    QList<Base::StatusBarContainerItem> tContainerSettings = method.getParseResult();
    for(int i = 0; i < tContainerSettings.size(); i++){
		Base::StatusBarContainerItem tContainerItem = tContainerSettings.at(i);

		Base::StatusBarContainer * tContainer = container(tContainerItem.m_id);
        if(tContainer){
            for(int j = 0;j < tContainerItem.m_items.size(); j++){
				Base::StatusBarItem tTmpItem = tContainerItem.m_items.at(j);
				Base::StatusBarItem tExistedItem = tContainer->item(tTmpItem.m_id);
                if(tExistedItem.m_widgetContainer && tExistedItem.m_action){
                    tExistedItem.m_action->setChecked(tTmpItem.m_bVisible);
                }
            }
        }
    }
}

/*!
* @brief 添加新容器
* @param[in] id 容器id
*/
void StatusBar::addContainer(Base::Id id)
{
    Q_D(StatusBar);
    if(existedItem(id))
        return;

	Base::StatusBarContainer * t_container = new Base::StatusBarContainer(id);
    connect(t_container,SIGNAL(actionCheckstateChanged(StatusBarContainer*)),this,SLOT(containerItemVisibleChanged(StatusBarContainer *)));

    d->m_rightContextMenu->appendGroup(id);

    d->m_itemLayout->addWidget(t_container->container());
    d->m_groups.append(t_container);
}

/*!
* @brief 在指定位置(beforedId)插入新的容器
* @param[in] beforedId 插入位置的id
* @param[in] id 带插入容器id
*/
void StatusBar::insertContainer(Base::Id beforeId, Base::Id id)
{
    Q_D(StatusBar);
    if(existedItem(id))
        return;

    if(existedItem(beforeId)){
        auto titer = std::find_if(d->m_groups.begin(),d->m_groups.end(),[&beforeId](Base::StatusBarContainer * container){return container->id() == beforeId;});
        for(int i = 0;i < d->m_itemLayout->count();i++){
            if(d->m_itemLayout->itemAt(i)->widget() == (*titer)->container()){
				Base::StatusBarContainer * t_container = new Base::StatusBarContainer(id);
                connect(t_container,SIGNAL(actionCheckstateChanged(StatusBarContainer*)),this,SLOT(containerItemVisibleChanged(StatusBarContainer *)));

                d->m_rightContextMenu->insertGroup(beforeId,id);
                d->m_groups.insert(i,t_container);
                d->m_itemLayout->insertWidget(i,t_container->container());
                break;
            }
        }
    }else{
        addContainer(id);
    }
}

/*!
* @brief 在指定容器中添加新item
* @param[in] contaienrId 待添加容器id
* @param[in] item 待添加item
*/
void StatusBar::appendItem(Base::Id containerId, Base::StatusBarItem item)
{
    Q_D(StatusBar);
    if(existedItem(containerId)){
		Base::StatusBarContainer * tContainer = container(containerId);
        if(tContainer){
            QAction * tAction = tContainer->addItem(item);
            if(tAction){
				Base::Action * act = Base::ActionManager::instance()->registAction(item.m_id,tAction);
                if(tContainer->size() == 1){
                    d->m_rightContextMenu->addSeparator(containerId);
                }
                d->m_rightContextMenu->addAction(act,containerId);
             }
        }
    }
}

void StatusBar::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    raise();
}

void StatusBar::contextMenuEvent(QContextMenuEvent *event)
{
    Q_D(StatusBar);
    d->m_rightContextMenu->menu()->exec(event->globalPos());
}

/*!
* @brief 单个容器中可见item数量改变
* @details 容器可见item数量改变后，需同步将可见状态保存至xml文件
* @param[in] container 发生改变的容器
*/
void StatusBar::containerItemVisibleChanged(Base::StatusBarContainer *container)
{
    Q_D(StatusBar);
    container->setVisible(container->visibleItemSize() != 0);

    if(d->m_delaySaveTimer == nullptr){
        d->m_delaySaveTimer = new QTimer();
        d->m_delaySaveTimer->setInterval(3000);
        QObject::connect(d->m_delaySaveTimer,SIGNAL(timeout()),this,SLOT(saveConfigXMLFile()));
    }

    d->m_delaySaveTimer->start();

}

void StatusBar::saveConfigXMLFile()
{
    Q_D(StatusBar);

    ProgramFilePath filepath;
    Base::RXmlFile tfile(filepath.statusbarConfigPath);
    StatusBarItemParseMethod method;
    method.setSaveContainers(d->m_groups);
    tfile.setParseMethod(&method,false);
    if(!tfile.startSave()){
        RLOG_ERROR("save statusbar config file error!");
    }

    d->m_delaySaveTimer->stop();
}

/*!
* @brief 判断是否存在对应id的项
* @param[in] id 待测试id项
* @return true:存在；false:不存在
*/
bool StatusBar::existedItem(Base::Id id)
{
    Q_D(StatusBar);
    auto titer = std::find_if(d->m_groups.begin(),d->m_groups.end(),[&id](Base::StatusBarContainer * container){return container->id() == id;});
    return titer != d->m_groups.end();
}

/*!
* @brief 获取指定id的容器
* @param[in] id 容器id
*/
Base::StatusBarContainer *StatusBar::container(Base::Id id)
{
    Q_D(StatusBar);
    auto titer = std::find_if(d->m_groups.begin(),d->m_groups.end(),[&id](Base::StatusBarContainer * container){return container->id() == id;});
    if(titer != d->m_groups.end())
        return *titer;
    return nullptr;
}

} //namespace Core
