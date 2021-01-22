/*!
 *  @brief     插件管理器
 *  @details   1.维护以MyDockWidget为原型的插件加载、初始化、卸载 @n
 *             2.管理MyTabWidget窗口 @n
 *             3.用于管理普通的窗口 @n
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <mutex>
#include <QObject>
#include <QMap>

#include <base/actionmanager/id.h>
#include <core/protocol/datastruct.h>

namespace Core{

class Widget;
class RComponent;
class RToolComponent;
class MyTabWidget;

class CORESHARED_EXPORT PluginManager : public QObject
{
    Q_OBJECT
public:
    static PluginManager * instance();

    struct PluginSpec{
        RComponent * pComp;     /*!< 实例化的插件 */
        Datastruct::ModuleInfo m_pluginModuleInfo;  /*!< 插件描述信息 */
    };

    typedef QMap<QString,Core::Widget*> WidgetMap;
    typedef QMap<Base::Id,RComponent *> ComponentMap;
    typedef QMap<Base::Id,PluginSpec> ActiveComponentMap;
    typedef QMap<QString,MyTabWidget*> TabMap;
    typedef QMap<Base::Id,RToolComponent *> ToolComponentMap;

    void load();

    //普通界面对话框
    void addNormalWidget(Core::Widget * widget);
    Core::Widget *getNormalWidget(QString objectName);
    WidgetMap getAllNormalWidgets();

    //可用插件
    void addAvailblePlugin(RComponent * component);
    RComponent* getAvailblePlugin(QString pluginId);
    ComponentMap getAllAvailblePlugins();

    //可用工具插件
    void addAvailbleToolPlugin(RToolComponent * component);
    ToolComponentMap getAllAvailbleToolPlugins();

    //被实例化激活插件
    void addActivePlugin(RComponent * component,Datastruct::ModuleInfo & info);
    RComponent * getActivePlugin(QString objectName);
    Datastruct::ModuleInfo getActivePluginModuleInfo(QString objectName, bool &existed);
    ActiveComponentMap &getAllActivePlugins();
    void releaseALL();

    //嵌入的tabwidget
    void addTabWidget(MyTabWidget * widget);
    bool removeWidget(QString objectName);
    MyTabWidget * getTabWidget(QString objectName);
    TabMap getAllTabWidgets();

public slots:
#ifndef OFFICAL_DOCK
    void ungroupDockWidget(QString tabObjName, QString dockObjName, QPoint mouseOffsetDistance);
    void hideDockWidget(QString tabObjName,QString dockObjName);
#endif

private slots:
#ifndef OFFICAL_DOCK
    void groupDockWidget(QString dockObjName,QPoint globalPos);
    void respDragState(Datastruct::DockDragState state);
    void rasieWidget(QString wid);
#endif

private:
    explicit PluginManager(QObject * parent = 0);
#ifndef OFFICAL_DOCK
    void addComponent(RComponent * activeComp, MyTabWidget * tabWidget);
    void removeComponent(RComponent *comp, QRect rect);
#endif

private:
    static PluginManager * m_instance;

    struct DragInfo{
        DragInfo(){
            reset();
        }

        void reset(){
            dragState = Datastruct::NoDrag;
            dragDockObjName.clear();
            mainGroupedComp = NULL;
        }
        QString dragDockObjName;                 /*!< 当前拖拽的dock的objname */
        Datastruct::DockDragState dragState;     /*!< dock窗口中移动的状态 */
        RComponent * mainGroupedComp;            /*!< 鼠标可放的窗口 */
    };

    std::mutex  m_mutex;
    ComponentMap availbleComponentMap;          /*!< 可用插件容器 */
    ActiveComponentMap activeComponentMap;      /*!< 已经激活插件容器 */

    std::mutex  m_toolMutex;
    ToolComponentMap availableToolComponentMap; /*!< 可用工具插件容器 */

    WidgetMap  noramlWidgetMap;                 /*!< 普通窗口容器 */
    TabMap  tabMap;                             /*!< 多dock组合成tab，保存tab容器 */

    DragInfo dragInfo;
};

} //namespace Base

#endif // PLUGINMANAGER_H
