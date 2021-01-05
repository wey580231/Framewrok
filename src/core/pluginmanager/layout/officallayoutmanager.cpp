#include "officallayoutmanager.h"

#include <QDebug>
#include <QAction>
#include <QDir>
#include <QFile>
#include <core/widgets/mainwindow.h>
#include "../../file/programfilepath.h"

namespace Core{

OfficalLayoutManager * OfficalLayoutManager::m_instance = nullptr;

OfficalLayoutManager::OfficalLayoutManager(QObject *parent):QObject(parent)
{

}

OfficalLayoutManager *OfficalLayoutManager::instance()
{
    if(m_instance == nullptr)
        m_instance = new OfficalLayoutManager();

    return m_instance;
}

/*!
 * @brief 添加布局项
 * @param[in] item 待添加布局item属性
 */
void OfficalLayoutManager::addDockItem(const Datastruct::OfficalDockItem &item)
{
    m_dockItems.append(item);
}

/*!
 * @brief 移除指定布局项
 * @param[in] groupId 布局项所属分组ID
 */
void OfficalLayoutManager::removeDockItem(QString groupId, QString typeId)
{
    auto fiter = std::remove_if(m_dockItems.begin(),m_dockItems.end(),[&](const Datastruct::OfficalDockItem & item){
        return item.m_groupId == groupId && item.m_typeId == typeId;
    });

    if(fiter != m_dockItems.end()){
        m_dockItems.erase(fiter);
    }
}

/*!
 * @brief 根据布局Id切换至对应布局
 * @param[in] layoutId Id唯一标识
 */
void OfficalLayoutManager::switchLayout(QString layoutId)
{
    if(m_layouts.contains(layoutId)){
        m_currentLayoutName = layoutId;
        m_layouts.value(layoutId)->startLayout(MainWindow::instance());
        importView();
    }
}

void OfficalLayoutManager::registLayout(AbstractOfficalLayout *layout)
{
    if(layout == nullptr || m_layouts.contains(layout->layoutId()))
        return;
    m_layouts.insert(layout->layoutId(),layout);

}

QList<QString> OfficalLayoutManager::registedLayouts()
{

    return m_layouts.keys();
}

const QList<Datastruct::OfficalDockItem> &OfficalLayoutManager::getRegistItems()
{
    return m_dockItems;
}

Datastruct::OfficalDockItem OfficalLayoutManager::getRegistItem(QString itemId)
{
    auto fresult = std::find_if(m_dockItems.begin(),m_dockItems.end(),[&itemId](const Datastruct::OfficalDockItem & item){return item.m_groupId == itemId;});
    if(fresult != m_dockItems.end())
        return *fresult;
    return Datastruct::OfficalDockItem();
}

/*!
 * @brief 绑定触发的按钮至布局
 * @param[in]   layoutName 布局名称
 * @param[in]   action 触发按钮
 */
void OfficalLayoutManager::bindActionToLayout(QString layoutName, QAction *action)
{
    if(m_layouts.contains(layoutName)){
        m_layouts.value(layoutName)->setAction(action);
    }
}

QString OfficalLayoutManager::getCurrentLayout()
{
    return m_currentLayoutName;
}

/*!
 * @brief 重置为为指定布局
 * @param[in] lastLayoutChoice 上一次保存的布局信息
 * @details 若存在上一次布局，则切换至上一次布局；否则重置为默认布局
 */
void OfficalLayoutManager::resetDefaultLayout(QString lastLayoutChoice)
{
    if(m_layouts.contains(lastLayoutChoice)){
        m_layouts.value(lastLayoutChoice)->triggeredAction()->setChecked(true);
        switchLayout(lastLayoutChoice);
    }else{
        if(m_layouts.size() > 0){
            m_layouts.first()->triggeredAction()->setChecked(true);
            switchLayout(m_layouts.firstKey());
        }
    }
}

/*!
 * @brief 根据当前选定的布局，在指定的布局保存路径下查找是否存在对应布局名称的文件；若存在则导入
 */
void OfficalLayoutManager::importView()
{
    ProgramFilePath filePath;
    QString saveFileName = filePath.layoutPath + QDir::separator() + m_currentLayoutName;
    QFile t_file(saveFileName);
    if(t_file.exists() && t_file.open(QFile::ReadOnly)){
        Core::MainWindow::instance()->restoreState(t_file.readAll());
    }
}

/*!
 * @brief 根据当前布局名称，导出布局
 */
void OfficalLayoutManager::exportView()
{
    ProgramFilePath filePath;
    QString saveFileName = filePath.layoutPath + QDir::separator() + m_currentLayoutName;
    QFile t_file(saveFileName);
    if(t_file.open(QFile::WriteOnly | QFile::Truncate)){
        t_file.write(Core::MainWindow::instance()->saveState());
        t_file.close();
    }
}

} //namespace Core
