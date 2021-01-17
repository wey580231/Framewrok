/*!
 *  @brief     通用树据模型
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.12.25
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef NAVIGATEMODEL_H
#define NAVIGATEMODEL_H

#include <QAbstractItemModel>

#include "../../base_global.h"

namespace Base {

/*!
 *  @brief 单个树节点
 *  @warning 创建节点时一定要设置节点的parentNode信息
 */
struct TreeNode{
    TreeNode():parentNode(NULL),nodeLevel(0),nodeData(NULL),nodeChecked(false){}

    int childIndex() const{
        if (parentNode)
            return parentNode->nodes.indexOf(const_cast<TreeNode*>(this));

        return 0;
    }

    int nodeId;
    QString nodeName;
    int nodeLevel;                  /*!< 树节点类型 （自定义类型,自行转换成需要的数据类型） >*/
    bool nodeChecked;               /*!< 是否被勾选(二级功能节点有效) */
    void * nodeData;                /*!< 节点绑定的数据 */

    TreeNode * parentNode;
    QList<TreeNode *> nodes;
};

class BASESHARED_EXPORT RTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit RTreeModel(QObject *parent = Q_NULLPTR);
    ~RTreeModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)Q_DECL_OVERRIDE;

    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QModelIndex parentIndexFromNode(TreeNode * node);
    QModelIndex indexFromNode(TreeNode* node);
    void refreshModel();

    void setHeaderData(QStringList headList);

    void addRootNode(TreeNode *node);
    void removeRootNode(TreeNode *node);

    int rootNodeSize();
    void clearModel();

    void setVirtualRootNode(TreeNode * node);
    TreeNode *virtualTreeRootNode(){return rootNode;}

protected:
    TreeNode * rootNode;           /*!< 虚拟根节点，不参与显示 */
    QStringList headList;          /*!< 头信息节点 */

};

} //namespace Base

#endif // NAVIGATEMODEL_H
