/*!
 *  @brief     通用树形视图
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.12.25
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TreeView_H
#define TreeView_H

#include <QTreeView>
#include <QSharedPointer>

#include "../../base_global.h"

class QMenu;

namespace Base {

class RTreeViewPrivate;

class BASESHARED_EXPORT RTreeView : public QTreeView
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(RTreeView)
public:
    explicit RTreeView(QWidget * parent = 0);
    ~RTreeView();

    void enableCustomContextMenuEvent(bool enabled);

    void resetRefreshModelState();

    void expandFirstLevel();

signals:
    void contextMenu(QPoint point);

private slots:
    void slotExpand(const QModelIndex &index);

    void respItemExpanded(QModelIndex index);
    void respItemCollapsed(QModelIndex index);

private:
    QSharedPointer<RTreeViewPrivate> d_ptr;

    QList<QModelIndex> m_expandedIndexs;        /*!< 被展开的索引集合 */
};

} //namespace Base

#endif // TreeView_H
