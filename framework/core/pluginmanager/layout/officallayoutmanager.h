/*!
 *  @brief     基于官方窗口布局管理器
 *  @details   1.只负责维护默认添加的几种布局，支持对应的切换
 *             2.保存所有已经添加的控件；
 *  @author    wey
 *  @version   1.0
 *  @date      2019.11.04
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef OFFICAL_LAYOUTMANAGER_H
#define OFFICAL_LAYOUTMANAGER_H

#include <QObject>
#include "../../core_global.h"
#include "../../protocol/datastruct.h"

class QMainWindow;

namespace Core{

/*!
 *  @brief 自定义布局
 *  @details 可根据OfficalLayoutManager提供的已存在的窗口项，自定义对布局进行排列设置。可通过菜单栏中布局选项进行统一的切换
 */
class CORESHARED_EXPORT AbstractOfficalLayout
{
public:
    explicit AbstractOfficalLayout():m_triggeredAction(nullptr){}

    virtual void startLayout(QMainWindow * window) = 0;
    virtual QString layoutId() = 0;

    void setAction(QAction * action){m_triggeredAction = action;}
    QAction * triggeredAction(){return m_triggeredAction;}

protected:
    QAction * m_triggeredAction;
};

class CORESHARED_EXPORT OfficalLayoutManager : public QObject
{
    Q_OBJECT
public:
    static OfficalLayoutManager * instance();
    void addDockItem(const Datastruct::OfficalDockItem & item);
    void removeDockItem(QString groupId, QString typeId);

    void registLayout(AbstractOfficalLayout * layout);

    QList<QString> registedLayouts();
    const QList<Datastruct::OfficalDockItem> & getRegistItems();
    Datastruct::OfficalDockItem getRegistItem(QString itemId);

    void bindActionToLayout(QString layoutName,QAction * action);

    QString getCurrentLayout();
    void resetDefaultLayout(QString lastLayoutChoice);

    void importView();
    void exportView();

public slots:
    void switchLayout(QString layoutId);

private:
    OfficalLayoutManager(QObject * parent = 0);

private:
    static OfficalLayoutManager * m_instance;
    QList<Datastruct::OfficalDockItem> m_dockItems;

    QMap<QString,AbstractOfficalLayout *> m_layouts;            /*!< key:布局Id,value:对应布局 */
    QString m_currentLayoutName;
};

}   //namespace Core

#endif // OFFICAL_LAYOUTMANAGER_H
