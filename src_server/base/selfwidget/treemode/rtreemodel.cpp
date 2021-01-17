#include "rtreemodel.h"

#include <QDebug>

namespace Base {

RTreeModel::RTreeModel(QObject *parent):QAbstractItemModel(parent)
{
    rootNode = new TreeNode();
}

RTreeModel::~RTreeModel()
{

}

QVariant RTreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());
    if(item == NULL)
        return QVariant();

    switch(role){
        case Qt::DisplayRole:
            {
                return item->nodeName;
            }
            break;
        default:break;
    }

    return QVariant();
}

Qt::ItemFlags RTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags fgs = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    return fgs;
}

QVariant RTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal){
        switch (role){
            case Qt::DisplayRole:return this->headList.at(section);
            case Qt::TextAlignmentRole: return Qt::AlignCenter;
            default:break;
        }
    }
    return QVariant();
}

QModelIndex RTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeNode * parentItem;

    if(!parent.isValid())
        parentItem = rootNode;
    else
        parentItem = static_cast<TreeNode *>(parent.internalPointer());

    TreeNode * childItem = parentItem->nodes.at(row);

    if(childItem)
        return createIndex(row,column,childItem);
    else
        return QModelIndex();
}

QModelIndex RTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();

    TreeNode * childItem = static_cast<TreeNode *>(index.internalPointer());
    TreeNode * parentItem = childItem->parentNode;

    if(parentItem == rootNode || parentItem == NULL)
        return QModelIndex();

    if(parentItem->parentNode == nullptr){
        return createIndex(rootNode->nodes.indexOf(parentItem),0,parentItem);
    }else{
        return createIndex(parentItem->childIndex(),0,parentItem);
    }
}

int RTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeNode * parentItem;
    if(parent.column() > 0)
        return 0;

    if(!parent.isValid())
        parentItem = rootNode;
    else
        parentItem = static_cast<TreeNode *>(parent.internalPointer());

    return parentItem->nodes.size();
}

int RTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return headList.size();
}

bool RTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.column() == 0){
        TreeNode * node = static_cast<TreeNode *>(index.internalPointer());
        if(node){
            switch(role){
                case Qt::CheckStateRole:{
                        node->nodeChecked = !node->nodeChecked;
                    }
                    break;
                case Qt::EditRole:
                    node->nodeName = value.toString();
                    break;
                default:
                    break;
            }
            return true;
        }
    }

    return QAbstractItemModel::setData(index,value,role);
}

/*!
 * @brief 更新模型数据源
 * @attention 每次对数据源操作后，务必调用此操作
 */
void RTreeModel::refreshModel()
{
    beginResetModel();

    endResetModel();
}

void RTreeModel::setHeaderData(QStringList headList)
{
    this->headList = headList;
}

/*!
 * @brief 添加根节点
 * @param[in]  rootNode 带添加的新节点
 */
void RTreeModel::addRootNode(TreeNode *node)
{
    if(node == NULL)
        return;

    rootNode->nodes.push_back(node);
}

/*!
 * @brief 根节点大小
 */
int RTreeModel::rootNodeSize()
{
    return rootNode->nodes.size();
}

/**
 * @brief 清空树形图
 */
void RTreeModel::clearModel()
{
    foreach(TreeNode * node,rootNode->nodes)
    {
        if(node)
        {
            removeRootNode(node);
        }
    }
    rootNode->nodes.clear();
}

/*!
 * @brief 替换现有的树形结构
 * @details 1.移除原有树形结构中所有的层级节点
 * @param[in]  node 新的虚拟节点
 */
void RTreeModel::setVirtualRootNode(TreeNode *node)
{
    if(rootNode){
        rootNode->nodes.clear();
        delete rootNode;
    }
    rootNode = node;
}

/*!
 * @brief 删除根节点
 * @param[in]  rootNode 带删除的节点
 */
void RTreeModel::removeRootNode(TreeNode *node)
{
    if(node == NULL)
        return;
    rootNode->nodes.removeOne(node);
}

/*!
 * @brief 通过数据节点反向获取指定索引的父节点
 * @param node 自定义树形节点的数据节点
 * @return 父节点
 */
QModelIndex RTreeModel::parentIndexFromNode(TreeNode *node)
{
    QModelIndex index = indexFromNode(node);
    if(index.isValid())
        return parent(index);

    return index;
}

/**
 * @brief 计算当前的行的ModelIndex
 * @param node 自定义树形节点
 * @return QModelIndex 对象
 */
QModelIndex RTreeModel::indexFromNode(TreeNode* node)
{
    if(node)
    {
        QList<TreeNode *> t_parentList;
        TreeNode *t_node = node;

        while(t_node)
        {
            t_parentList.insert(0,t_node);

            if(t_node->parentNode == nullptr)
                break;

            t_node = t_node->parentNode;
        }

        QModelIndex nodeIndex = QModelIndex();
        for(int i = 0; i < t_parentList.count(); i++){
            if(t_parentList.at(i)->parentNode == nullptr){
                nodeIndex = index(rootNode->nodes.indexOf(t_parentList.at(i)),0,nodeIndex);
            }else{
                nodeIndex = index(t_parentList.at(i)->childIndex(),0,nodeIndex);
            }
        }

        return nodeIndex ;
    }

    return QModelIndex();
}

} //namespace Base
