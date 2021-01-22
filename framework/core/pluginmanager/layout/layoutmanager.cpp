#include "layoutmanager.h"

#include <QDir>
#include <QFileDialog>

#include <base/selfwidget/rmessagebox.h>
#include <base/util/rsingleton.h>
#include <base/util/rutil.h>
#include <core/selfwidget/mytabwidget.h>
#include <core/selfwidget/widget.h>
#include <core/protocol/datastruct.h>
#include <core/widgets/mainwindow.h>
#include <core/widgets/mainwindow.h>
#include "layoutparsemethod.h"
#include "../../file/programfilepath.h"
#include "../plugin/pluginmanager.h"
#include "../interface/rcomponent.h"

#include <QDebug>

namespace Core{

LayoutManager::LayoutManager(QObject *parent):QObject(parent)
{

}

/*!
 * @brief 加载config/layouts目录下所有可用的布局
 * @return 返回可用的布局文件名集合
 */
QStringList LayoutManager::localLayoutFilenames()
{
    ProgramFilePath filePath;
    QDir dir(filePath.layoutPath);
    QFileInfoList t_fileList = dir.entryInfoList(QStringList(),QDir::Files | QDir::NoDotAndDotDot);
    QStringList results;
    std::for_each(t_fileList.begin(),t_fileList.end(),[&results](QFileInfo & info){
        results.append(info.baseName());
    });
    return results;
}

/*!
 * @brief 导入指定布局文件
 * @param[in]   fileName 待导入布局文件名
 */
bool LayoutManager::importView(QString fileName)
{
    Core::LayoutParseMethod method;
    Base::RTextFile file(fileName);
    file.setParseMethod(&method,false);
    if(file.startParse())
    {
        //[1]
        QList<Datastruct::DockItem *> dockItems = method.getDockItems();
        QList<Datastruct::DockItem *>::iterator diter = dockItems.begin();

        QSize mainScreenSize = Base::RUtil::screenSize();

        auto adjustWidgetGeometry = [&mainScreenSize](QWidget * widget,QRect & layoutGeometry){
            if(layoutGeometry.right() > mainScreenSize.width() || layoutGeometry.bottom() > mainScreenSize.height()){
                int x = (mainScreenSize.width() - layoutGeometry.width()) / 2;
                int y = (mainScreenSize.height() - layoutGeometry.height()) / 2;
                widget->setGeometry(QRect(QPoint(x,y),layoutGeometry.size()));
            }else{
                widget->setGeometry(layoutGeometry);
            }
        };

        while(diter != dockItems.end()){
            Datastruct::DockItem * dockItem = *diter;
            Core::Widget * widget = PluginManager::instance()->getNormalWidget(dockItem->objName);

            if(widget){
                //20190327 普通窗口(标题栏和右侧图层栏)不设置尺寸
//                widget->setGeometry(dockItem->geometry);
                widget->setExpanded(dockItem->expanded);
                widget->setWidgetFeatures(static_cast<Core::Widget::WidgetFeature>(dockItem->feature));
            }else{
#ifndef OFFICAL_DOCK
                RComponent * comp = PluginManager::instance()->getActivePlugin(dockItem->objName);
                if(comp){
                    MyDockWidget * dockWidget = comp->getDockContainer();
                    adjustWidgetGeometry(dockWidget,dockItem->geometry);
                    dockWidget->setExpanded(dockItem->expanded);
                    dockWidget->setTabInfo(dockItem->tabInfo);
                    dockWidget->setWidgetFeatures(static_cast<Core::Widget::WidgetFeature>(dockItem->feature));
                }
#endif
            }
            diter++;
        }

        //[2]
        QList<Datastruct::TabItem *> tabItems = method.getTabItems();

        //!模拟
//        Datastruct::TabItem * item = new Datastruct::TabItem;
//        item->currentIndex = 0;
//        item->feature = Widget::AllWidgetFeatures | Widget::WidgetRangeLimit;
//        item->geometry = QRect(QPoint(500,400),QSize(500,450));
//        item->objName = QString("Hello11111");
//        item->tabPageObjNames.append("Plugin.TaskControl.TaskControl_2");
//        item->tabPageObjNames.append("Plugin.TaskControl.TaskControl_3");
//        tabItems.append(item);
        //!模拟

#ifndef OFFICAL_DOCK
        QList<Datastruct::TabItem *>::iterator titer = tabItems.begin();
        while(titer != tabItems.end()){
            Datastruct::TabItem * tabItem = *titer;
            QStringList objs = tabItem->tabPageObjNames;

            QList<RComponent * > subComps;
            foreach(QString tabObjName,objs){
                RComponent * activeComp = PluginManager::instance()->getActivePlugin(tabObjName);
                if(activeComp){
                    subComps.append(activeComp);
                }
            }

            if(subComps.size() > 1){
                MyTabWidget * tabWidget = new MyTabWidget(MainWindow::instance());
                tabWidget->setObjectName(tabItem->objName);
                tabWidget->setTabPosition(static_cast<MyTabWidget::TabPosition>(tabItem->tabPosition));
                adjustWidgetGeometry(tabWidget,tabItem->geometry);
                foreach(RComponent * activeComp,subComps){
                    tabWidget->addTab(activeComp->getDockContainer(),activeComp->name());
                    connect(activeComp->getDockContainer(),SIGNAL(ungroupDockWidget(QString,QString,QPoint)),PluginManager::instance(),SLOT(ungroupDockWidget(QString,QString,QPoint)));
                    activeComp->getDockContainer()->addWidgetFeatures(Widget::WidgetTabbed);
                }

                tabWidget->setCurrentIndex(tabItem->currentIndex);
                tabWidget->setWidgetFeatures(static_cast<Widget::WidgetFeatures>(tabItem->feature));
                PluginManager::instance()->addTabWidget(tabWidget);
                tabWidget->show();
            }
            titer++;
        }
#endif
        return true;
    }
    return false;
}

/*!
 * @brief 检测默认路径下是否存在对应文件
 * @param[in]  fileName 待测试文件名
 */
bool LayoutManager::existed(QString fileName)
{
    if(fileName.isEmpty())
        return false;

    ProgramFilePath filePath;
    QString t_sz_fullFilePath = filePath.layoutPath + QDir::separator() + fileName;
    QFileInfo t_fileInfo(t_sz_fullFilePath);
    if(t_fileInfo.exists() && t_fileInfo.isFile())
        return true;

    return false;
}

/*!
 * @brief 重置当前布局为默认布局
 * @details 当指定的布局文件不存在时调用
 */
void LayoutManager::resotreDefaultLayout()
{
    ProgramFilePath filePath;
    m_sz_layoutFileName = QFileInfo(filePath.layoutFile).baseName();
}

QString LayoutManager::getCurrentLayout()
{
    if(m_sz_layoutFileName.isEmpty())
        resotreDefaultLayout();

    return m_sz_layoutFileName;
}

void LayoutManager::setCurrentLayout(QString fileName)
{
    this->m_sz_layoutFileName = fileName;
}

/*!
 * @brief 导入布局
 * @details 根据ObjName查找widget，则从文件中将信息设置回各个显示控件 @n
 *          [1]：设置普通Widget和MyDockWidget @n
 *          [2]：设置MyTabWidget @n
 * @warning 先设置组件的一般信息，最后设置features。
 */
void LayoutManager::importView()
{
    ProgramFilePath filePath;
    importView(filePath.layoutPath + QDir::separator() + getCurrentLayout()/* + ".bin"*/);
}

/*!
 * @brief 保存布局
 * @details 保存内容：1.DockItem; 2.TabItem  @n
 *          保存路径：config/layout.bin @n
 *          保存控件范围：MyDockWidget；MyTabWidget；上部菜单栏；右侧图层控制栏 @n
 * @param[in] showOperateResult 为true时，显示导出结果
 */
void LayoutManager::exportView(bool showOperateResult)
{
    ProgramFilePath filePath;
    QString saveFileName;
    if(!showOperateResult){
        saveFileName = QFileDialog::getSaveFileName(MainWindow::instance(),tr("choose file"),filePath.layoutPath/*,"*.bin"*/);
        if(saveFileName.isEmpty())
            return;
    }else{
        saveFileName = filePath.layoutPath + QDir::separator() + getCurrentLayout()/* + ".bin"*/;
    }

    LayoutParseMethod method;

    Base::RTextFile file(saveFileName);
    file.setParseMethod(&method,false);

    //[1]保存普通widget
    PluginManager::WidgetMap widgets = PluginManager::instance()->getAllNormalWidgets();
    PluginManager::WidgetMap::iterator witer = widgets.begin();
    while(witer != widgets.end()){
        Core::Widget * wid = witer.value();
        Datastruct::DockItem * item = new Datastruct::DockItem;
        item->objName = wid->objectName();
        item->geometry = wid->geometry();
        item->feature = static_cast<int>(wid->getWidgetFeatures());
        item->expanded = wid->getExpanded();
        method.addDockItem(item);
        witer++;
    }
#ifndef OFFICAL_DOCK
    //[2]保存plugin
    PluginManager::ActiveComponentMap plugins = PluginManager::instance()->getAllActivePlugins();
    PluginManager::ActiveComponentMap::iterator piter = plugins.begin();
    while(piter != plugins.end()){
        RComponent * comp = piter.value().pComp;
        MyDockWidget * dock = comp->getDockContainer();
        Datastruct::DockItem * item = new Datastruct::DockItem;
        item->objName = dock->objectName();
        item->geometry = dock->geometry();
        item->feature = static_cast<int>(dock->getWidgetFeatures());
        item->expanded = dock->getExpanded();
        item->tabInfo  = dock->getTabInfo();
        method.addDockItem(item);
        piter++;
    }

    //[3]保存tabwidet
    PluginManager::TabMap tabs = PluginManager::instance()->getAllTabWidgets();
    PluginManager::TabMap::iterator titer = tabs.begin();
    while(titer != tabs.end()){
        MyTabWidget * tab = titer.value();
        Datastruct::TabItem * item = new Datastruct::TabItem;
        item->objName = tab->objectName();
        item->geometry = tab->geometry();
        item->feature = static_cast<int>(tab->getWidgetFeatures());
        item->tabPosition = static_cast<int>(tab->getTabPosition());
        item->currentIndex = tab->currentIndex();
        item->tabPageObjNames = tab->tabIds();
        method.addTabItem(item);
        titer++;
    }
#endif
    //TODO 可增加状态显示栏，减少弹窗使用
    bool result = file.startSave(QFile::WriteOnly);
    if(!showOperateResult){
        if(result){
			Base::RMessageBox::information(MainWindow::instance(),tr("information"),tr("export layout successfully!"), Base::RMessageBox::Yes);
        }else{
			Base::RMessageBox::warning(MainWindow::instance(),tr("warning"),tr("export layout failed!"), Base::RMessageBox::Yes);
        }
    }
}

}
