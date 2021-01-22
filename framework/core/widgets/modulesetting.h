/*!
 *  @brief     模块设置面板
 *  @details   1.可对组件的状态进行设置;
 *             2.可对网络信息设置;
 *             3.可查看组件的详细信息(协议信息、网络配置信息)
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.14
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef MODULESETTING_H
#define MODULESETTING_H

#include <QAbstractItemModel>
#include <QMap>

#include <base/selfwidget/dialogproxy.h>
#include <core/selfwidget/widget.h>
#include <core/pluginmanager/mailbox/observer.h>
#include "../protocol/datastruct.h"

class QTreeView;

namespace Core{

class MSettingModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MSettingModel(QTreeView * view,QObject *parent = 0);
    ~MSettingModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)Q_DECL_OVERRIDE;

    QString getNodeId(int parentIndex,int childIndex);

public:
    struct TreeNode{
        TreeNode():index(0),parentNode(NULL),visible(true){}
        QString nodeName;
        QString nodeId;
        QString objName;
        int index;
        bool visible;
        TreeNode * parentNode;
        QList<TreeNode *> nodes;
    };

signals:
    void moduleStateChanged(MSettingModel::TreeNode * node);

private slots:
    void userTriggered(bool triggered);

private:
    void udpateModel();

private:
    TreeNode * rootNode;
    QMap<QString,TreeNode*> nodeMap;
    QStringList headList;
    QTreeView * treeView;
};

class ModuleSettingPrivate;

class ModuleSetting : public Widget,public Observer
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModuleSetting)
public:
    explicit ModuleSetting(QWidget * parent = 0);
    ~ModuleSetting();

    QSize sizeHint()const;

    void onMessage(QString messageType);

signals:
    void raiseWidget(QString wid);

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void mousePressEvent(QMouseEvent * event);
    void updateFeatures();

private slots:
    void updatePluginState(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    void updateModuelState(MSettingModel::TreeNode *node);
    void raiseModule(QModelIndex index);
    void contextMenu(QPoint);
    void viewModuleDetailInfo(bool);

private:
    void animationView(bool isVisible);

private:
    ModuleSettingPrivate * d_ptr;
};

/*!
 *  @brief 集成QML显示模块的详细信息
 *  @details 此类是为了探索与QML结合，而做的一个demo。希望通过此引入QML的优秀交互效果。20181130 mark一下
 */
class QmlModuleDetailDialogPrivate;

class QmlModuleDetailDialog : public Base::DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QmlModuleDetailDialog)
public:
    explicit QmlModuleDetailDialog(QWidget *parent = 0);
    ~QmlModuleDetailDialog();

    QSize sizeHint()const;

private:
    QSharedPointer<QmlModuleDetailDialogPrivate> d_ptr;

};

}

#endif // MODULESETTING_H
