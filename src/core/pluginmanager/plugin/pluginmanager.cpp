#include "pluginmanager.h"

#include <base/util/rsingleton.h>
#include <core/widgets/mainwindow.h>
#include <core/selfwidget/mytabwidget.h>
#include <core/selfwidget/mydockwidget.h>
#include "../interface/rcomponent.h"
#include "../interface/rtoolcomponent.h"
#include "pluginloader.h"

#include <QDebug>

namespace Core{

PluginManager * PluginManager::m_instance = nullptr;

PluginManager::PluginManager(QObject *parent):
    QObject(parent)
{

}

PluginManager *PluginManager::instance()
{
    if(m_instance == nullptr){
        m_instance = new PluginManager();
    }
    return m_instance;
}

void PluginManager::load()
{
#ifdef ENABLE_BUSINESS_PLUGIN
    PluginLoader::instance()->loadPlugin();
#endif

#ifdef ENABLE_TOOL_PLUGIN
    PluginLoader::instance()->loadToolPlugin();
#endif
}

void PluginManager::addNormalWidget(Core::Widget *widget)
{
    if(noramlWidgetMap.contains(widget->objectName()))
        return;
    noramlWidgetMap.insert(widget->objectName(),widget);
}

Core::Widget* PluginManager::getNormalWidget(QString objectName)
{
    if(noramlWidgetMap.contains(objectName))
        return noramlWidgetMap.value(objectName);
    return NULL;
}

PluginManager::WidgetMap PluginManager::getAllNormalWidgets()
{
    return noramlWidgetMap;
}

/*!
 * @brief 添加可用的插件
 * @details 插件只创建，但未实例化
 * @param[in]  component 待添加的插件
 */
void PluginManager::addAvailblePlugin(RComponent *component)
{
    if(component == NULL)
        return;

    std::lock_guard<std::mutex> lg(m_mutex);
    if(availbleComponentMap.contains(component->id()))
        return;

    availbleComponentMap.insert(component->id(),component);
}

/*!
 * @brief 根据插件ID查询是否存在此插件信息
 * @details 插件ID用以标识每个插件的身份信息，由插件按照顺序自己指定，不可重复。
 * @param[in]  pluginId 待查询的插件唯一标识
 * @return 返回对应的插件或返回NULL
 */
RComponent *PluginManager::getAvailblePlugin(QString pluginId)
{
    std::lock_guard<std::mutex> lg(m_mutex);
    ComponentMap::iterator iter = availbleComponentMap.begin();
    while(iter != availbleComponentMap.end()){
        if(iter.value()->getPluginId() == pluginId)
            return iter.value();
        iter++;
    }

    return NULL;
}

PluginManager::ComponentMap PluginManager::getAllAvailblePlugins()
{
    std::lock_guard<std::mutex> lg(m_mutex);
    return availbleComponentMap;
}

void PluginManager::addAvailbleToolPlugin(RToolComponent *component)
{
    if(component == NULL)
        return;

    std::lock_guard<std::mutex> lg(m_toolMutex);
    if(availableToolComponentMap.contains(component->id()))
        return;

    availableToolComponentMap.insert(component->id(),component);
}

PluginManager::ToolComponentMap PluginManager::getAllAvailbleToolPlugins()
{
    std::lock_guard<std::mutex> lg(m_toolMutex);
    return availableToolComponentMap;
}

/*!
 * @brief 根据配置文件，将可用的插件添加至待激活的区
 * @param[in]  component 待添加的插件
 * @param[in]  info 插件配置信息，对等于plugins.xml中moudle节点
 */
void PluginManager::addActivePlugin(RComponent *component, Datastruct::ModuleInfo &info)
{
    if(component == NULL)
        return;

    std::lock_guard<std::mutex> lg(m_mutex);
    if(activeComponentMap.contains(component->id()))
        return;

    PluginSpec pluginSpec;
    pluginSpec.pComp = component;
    pluginSpec.m_pluginModuleInfo = info;
    activeComponentMap.insert(component->id(),pluginSpec);
}

RComponent * PluginManager::getActivePlugin(QString objectName)
{
    std::lock_guard<std::mutex> lg(m_mutex);
    ActiveComponentMap::iterator iter = activeComponentMap.begin();
    while(iter != activeComponentMap.end()){
        if(iter.value().pComp->id().toString() == objectName)
            return iter.value().pComp;
        iter++;
    }

    return NULL;
}

/*!
 * @brief 获取激活控件的配置信息
 * @param[in]  objectName 激活控件objName
 * @param[in/out]  existed 是否存在指定激活控件的配置信息
 * @return  控件对应的配置信息，existed为true时，返回所查询的配置信息；否则无意义
 */
Datastruct::ModuleInfo PluginManager::getActivePluginModuleInfo(QString objectName, bool & existed)
{
    existed = false;
    std::lock_guard<std::mutex> lg(m_mutex);
    ActiveComponentMap::iterator iter = activeComponentMap.begin();
    while(iter != activeComponentMap.end()){
        if(iter.value().pComp->id().toString() == objectName){
            existed = true;
            return iter.value().m_pluginModuleInfo;
        }
        iter++;
    }
    return Datastruct::ModuleInfo();
}

PluginManager::ActiveComponentMap & PluginManager::getAllActivePlugins()
{
    std::lock_guard<std::mutex> lg(m_mutex);
    return activeComponentMap;
}

/*!
 * @brief 释放所有的插件
 */
void PluginManager::releaseALL()
{
    std::lock_guard<std::mutex> lg(m_mutex);
    ActiveComponentMap::iterator iter = activeComponentMap.begin();
    while(iter != activeComponentMap.end()){
        iter.value().pComp->release();
        delete iter.value().pComp;
        iter++;
    }
    activeComponentMap.clear();
}

void PluginManager::addTabWidget(MyTabWidget *widget)
{
    if(tabMap.contains(widget->objectName()))
        return;
    tabMap.insert(widget->objectName(),widget);
}

bool PluginManager::removeWidget(QString objectName)
{
    return tabMap.remove(objectName) >= 1;
}

MyTabWidget *PluginManager::getTabWidget(QString objectName)
{
    if(tabMap.contains(objectName))
        return tabMap.value(objectName);
    return NULL;
}

PluginManager::TabMap PluginManager::getAllTabWidgets()
{
    return tabMap;
}

#ifndef OFFICAL_DOCK

/*!
 * @brief 在指定的tab中隐藏指定的dock窗口
 * @details 待隐藏的窗口从tab中移除，但需要再显示时还可以在tab中恢复显示。
 * @param[in] tabObjName  组合的tab的id
 * @param[in] dockObjName 主动取消组合的dock的id
 */
void PluginManager::hideDockWidget(QString tabObjName, QString dockObjName)
{
    MyTabWidget * tabWidget = getTabWidget(tabObjName);
    if(tabWidget){
        tabWidget->hideTab(dockObjName);
    }
}

/*!
 * @brief 取消当前dock的组合
 * @details 1.若当前组合产生的tabwidget的子tab数量大于2，取消组合时只需要从移除即可; @n
 *          2.若当前组合的tabwidget的子tab数量等于2，取消组合后，需要将各个子dock独立，并且移除tabwidget @n
 * @param[in] tabObjName  组合的tab的id
 * @param[in] dockObjName 主动取消组合的dock的id
 */
void PluginManager::ungroupDockWidget(QString tabObjName, QString dockObjName,QPoint mouseOffsetDistance)
{
    MyTabWidget * tabWidget = getTabWidget(tabObjName);
    if(tabWidget){
        QRect tabRect = tabWidget->contentGeometry();
        QStringList tabIds = tabWidget->tabIds();

        if(tabWidget->removeTab(dockObjName)){
            tabIds.removeOne(dockObjName);
            RComponent * triggeredComp = getActivePlugin(dockObjName);
            QRect topRect = tabRect;

            topRect.moveTopLeft(topRect.topLeft() + mouseOffsetDistance);
            removeComponent(triggeredComp,topRect);

            if(tabWidget->tabsCount() == 1 && tabIds.size() == 1){
                tabWidget->removeTab(tabIds.first());
                MyDockWidget * dock = getActivePlugin(tabIds.first())->getDockContainer();
                removeComponent(getActivePlugin(tabIds.first()),tabRect);

                if(removeWidget(tabObjName))
                    delete tabWidget;

                dock->move(tabRect.topLeft());
            }
            triggeredComp->getDockContainer()->raise();
        }
    }
}

/*!
 * @brief 组合拖动dock控件
 * @details 进行窗口组合时，存在1、2两种拖动情形:
 *          1.A dock移动至B dock上，组合成全新的tab；
 *          2.A dock移动至A tab上，添加新的tab；
 *          3.A tab移动至B tab上，组合成新tab(待优化)；
 * @param[in] dockObjName 当前待嵌入dock中objname
 * @param[in] globalPos 当前鼠标的全局位置
 */
void PluginManager::groupDockWidget(QString dockObjName, QPoint globalPos)
{
    ActiveComponentMap::iterator diter = activeComponentMap.begin();

    RComponent * tmpComp = NULL;

    while(diter != activeComponentMap.end()){
        PluginSpec spec = diter.value();
        if(spec.pComp->objectName() != dockObjName){
            MyDockWidget * dock = spec.pComp->getDockContainer();
            if(dock->isVisible()){
                if(dock->containsGlobal(globalPos)){
                    tmpComp = spec.pComp;
                    break;
                }else{
                    Datastruct::TabInfo tabInfo = dock->getTabInfo();
                    if(tabInfo.tabbed){
                        MyTabWidget * tabWidget = getTabWidget(tabInfo.tabObjName);
                        if(tabWidget && tabWidget->geometry().contains(globalPos)){
                            //由tabwidget来将坐标进行转换成内部坐标
                            QPoint tabLocalPoint = tabWidget->mapFromGlobal(globalPos);
                            QWidget * curTab = tabWidget->currentWidget();
                            if(curTab){
                                MyDockWidget * dock = dynamic_cast<MyDockWidget *>(curTab);
                                if(dock && dock->containsLocalPoint(tabLocalPoint)){
                                    tmpComp = getActivePlugin(dock->objectName());
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        diter++;
    }

    if(tmpComp != dragInfo.mainGroupedComp){
        if(dragInfo.mainGroupedComp)
            dragInfo.mainGroupedComp->getDockContainer()->setDragState(Datastruct::DragLeave);

        dragInfo.dragDockObjName = dockObjName;
        dragInfo.mainGroupedComp = tmpComp;
        if(tmpComp != NULL)
            dragInfo.mainGroupedComp->getDockContainer()->setDragState(Datastruct::DragEnter);
    }
}

/*!
 * @brief 响应dock窗体拖拽的状态变化
 * @details 1.从DragMove转换到Drop状态的同时，当前鼠标下有可放的区域，则创建组合窗体: @n
 *              a.若可放dock未嵌入，则由拖动的dock与可放的dock组合成tab； @n
 *              b.若客房的cok已嵌入，则由拖动的dock加入可放置dock的tabwdiget。
 *          2.新创建的TabWidget窗口的位置由被拖入的的控件决定 @n
 * @param[in] state 拖拽状态
 */
void PluginManager::respDragState(Datastruct::DockDragState state)
{
    if(dragInfo.dragState == Datastruct::DragMove && dragInfo.mainGroupedComp){
        if(state == Datastruct::DragTerminate){
            dragInfo.mainGroupedComp->getDockContainer()->setDragState(Datastruct::DragLeave);
            dragInfo.mainGroupedComp = NULL;
        }else if(state == Datastruct::Drop){
            Datastruct::TabInfo tabInfo = dragInfo.mainGroupedComp->getDockContainer()->getTabInfo();
            if(tabInfo.tabbed){
                MyTabWidget * tabWidget = getTabWidget(tabInfo.tabObjName);
                addComponent(getActivePlugin(dragInfo.dragDockObjName),tabWidget);
                dragInfo.mainGroupedComp->getDockContainer()->setDragState(Datastruct::DragLeave);
            }else{
                QList<RComponent *> subComps;
                subComps.push_back(dragInfo.mainGroupedComp);
                subComps.push_back(getActivePlugin(dragInfo.dragDockObjName));

                MyTabWidget * tabWidget = new MyTabWidget(MainWindow::instance());
                QRect originRect = dragInfo.mainGroupedComp->getDockContainer()->geometry();
                QRect newRect = tabWidget->setContentGeomtry(originRect);

                tabWidget->setGeometry(newRect);

                foreach(RComponent * activeComp,subComps){
                    addComponent(activeComp,tabWidget);
                }
                addTabWidget(tabWidget);
                tabWidget->show();
            }
            dragInfo.reset();
        }
    }else{
        dragInfo.dragState = state;
    }
}


/*!
 * @brief 提升组件显示层次
 * @details 1.若当前dock未嵌入，则直接提升显示; @n
 *          2.若dock嵌入，则设置其在tabWidget中置顶显示 @n
 * @param[in] wid plugin的objectName
 */
void PluginManager::rasieWidget(QString wid)
{
    RComponent * comp = getActivePlugin(wid);
    if(comp){
        Datastruct::TabInfo tabInfo = comp->getDockContainer()->getTabInfo();
        if(tabInfo.tabbed){
            MyTabWidget * tabWidget = getTabWidget(tabInfo.tabObjName);
            if(tabWidget){
                tabWidget->setCurrentWidget(comp->getDockContainer());
            }
        }else{
            comp->getDockContainer()->raise();
        }
    }
}

/*!
 * @brief 向tabWidget中添加tab页(activeComp)
 * @param[in] activeComp 待添加插件
 * @param[in] tabWidget tab容器
 */
void PluginManager::addComponent(RComponent * activeComp,MyTabWidget * tabWidget)
{
    MyDockWidget * dock = activeComp->getDockContainer();
    dock->setDragState(Datastruct::DragLeave);
    dock->move(0,0);
    tabWidget->addTab(dock,activeComp->name());
    tabWidget->setCurrentIndex(tabWidget->tabsCount() - 1);
    connect(dock,SIGNAL(ungroupDockWidget(QString,QString,QPoint)),PluginManager::instance(),SLOT(ungroupDockWidget(QString,QString,QPoint)));

    Datastruct::TabInfo info;
    info.tabbed = true;
    info.tabObjName = tabWidget->objectName();
    dock->setTabInfo(info);
    dock->removeWidgetFeatures(Widget::WidgetDialog);
    dock->addWidgetFeatures(Widget::WidgetTabbed);
}

void PluginManager::removeComponent(RComponent * comp,QRect rect)
{
    if(comp){
        MyDockWidget * dock = comp->getDockContainer();
        dock->setParent(MainWindow::instance());
        dock->setGeometry(rect);
        dock->removeWidgetFeatures(Widget::WidgetTabbed);
        dock->addWidgetFeatures(Widget::WidgetDialog);
        dock->setVisible(true);

        Datastruct::TabInfo info;
        dock->setTabInfo(info);
    }
}

#endif

} //namespace Base
