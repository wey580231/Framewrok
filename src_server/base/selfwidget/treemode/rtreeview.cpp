#include "rtreeview.h"

#include <QMenu>
#include <QAction>
#include <QDebug>

namespace Base {

class RTreeViewPrivate{
    Q_DECLARE_PUBLIC(RTreeView)
private:
    RTreeViewPrivate(RTreeView * q):q_ptr(q){
        initView();
    }

    void initView();

    RTreeView * q_ptr;
};

void RTreeViewPrivate::initView()
{

}

RTreeView::RTreeView(QWidget *parent):QTreeView(parent),d_ptr(new RTreeViewPrivate(this))
{
//    connect(this,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(slotExpand(const QModelIndex &)));

    connect(this,SIGNAL(collapsed(QModelIndex)),this,SLOT(respItemCollapsed(QModelIndex)));
    connect(this,SIGNAL(expanded(QModelIndex)),this,SLOT(respItemExpanded(QModelIndex)));
}

RTreeView::~RTreeView()
{

}

/*!
 * @brief 设置右键树形菜单
 * @details 只有在开启此功能之后，在产生右键事件时，才发出contextMenu(QPoint)信号
 * @param[in]  enabled 开启自定义右键事件
 */
void RTreeView::enableCustomContextMenuEvent(bool enabled)
{
    if(enabled){
        setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SIGNAL(contextMenu(QPoint)));
    }else{
        setContextMenuPolicy(Qt::DefaultContextMenu);
        disconnect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SIGNAL(contextMenu(QPoint)));
    }
}

/*!
 * @brief 重置树形控件展开状态至数据刷新前状态
 */
void RTreeView::resetRefreshModelState()
{
    for(QModelIndex index : m_expandedIndexs){
        setExpanded(index,true);
    }
}

/**
 * @brief 设置展开还是折叠选中的单元格
 * @param index 单元格
 */
void RTreeView::slotExpand(const QModelIndex& index)
{
    if(isExpanded(index))
        collapse(index);
    else
        expand(index);
}

/*!
 * @brief 响应Item展开信息
 * @param[in] index 被点击展开的Item信息
 */
void RTreeView::respItemExpanded(QModelIndex index)
{
    auto iter = std::find_if(m_expandedIndexs.begin(),m_expandedIndexs.end(),[&index](QModelIndex eindex){return eindex == index;});
    if(iter == m_expandedIndexs.end())
        m_expandedIndexs.push_back(index);
}

/*!
 * @brief 响应Item折叠信息
 * @param[in] index 被点击折叠的Item信息
 */
void RTreeView::respItemCollapsed(QModelIndex index)
{
    auto iter = std::remove_if(m_expandedIndexs.begin(),m_expandedIndexs.end(),[&index](QModelIndex eindex){return eindex == index;});
    if(iter != m_expandedIndexs.end()){
        m_expandedIndexs.erase(iter,m_expandedIndexs.end());
    }
}

/*!
 * @brief 展示一级根节点
 */
void RTreeView::expandFirstLevel()
{
    QAbstractItemModel * mod = model();
    int rows = mod->rowCount();

    for(int i = 0; i < rows; i++){
        expand(mod->index(i,0));
    }
}

} //namespace Base
