/*!
 *  @brief     网络控制工具栏
 *  @details   提供一键开启、关闭网络，并支持启动时自动开启网络等设置
 *  @author    wey
 *  @version   1.0
 *  @date      2019.09.10
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef NETSTATECONTROLTOOL_H
#define NETSTATECONTROLTOOL_H

#include "abstractstatustool.h"
#include "../../selfwidget/widget.h"
#include "../../protocol/datastruct.h"

class QPushButton;\
class QToolButton;
class QLabel;
class QMenu;
class QAction;
class QTreeWidget;
class QTreeWidgetItem;
class QTimer;

namespace Core{

class PacketDetailWidget : public Widget
{
    Q_OBJECT
public:
    explicit PacketDetailWidget(QWidget * parent = 0);
    ~PacketDetailWidget();

protected:
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);
    void showEvent(QShowEvent * event);
    void hideEvent(QHideEvent * event);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void intView();

private slots:
    void updateTreeItemData();
    void respContextMenu(QPoint point);
    void clearNetStaticInfo(bool flag);

private:
    enum TreeColumn{
        T_Name,
        T_Type,
        T_Count,
    };

private:
    QWidget * m_mainWidget;
    QTreeWidget * m_treeWidget;

    QMap<QString,QTreeWidgetItem *> m_treeItems;            /*!< 属性表格中二级节点集合，key:网卡名_协议类型 */
    QTimer * m_refreshTimer;

    QMenu * m_contextMenu;
};

class NetStateControlTool : public AbstractStatusTool
{
    Q_OBJECT
public:
    explicit NetStateControlTool(QWidget * parent = 0);
    ~NetStateControlTool();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void generateSingleLayout();
    void initConnect();
    void updateStatusLabel();

private slots:
    void respStatic();
    void updateNetState();
    void respAutoStartNetwork(bool flag);
    void respDebugNetwork(bool flag);

private:
    QString m_configKey;

    QToolButton * m_startButt;          /*!< 开启网络 */
    QToolButton * m_stopButt;           /*!< 关闭网络 */
    QToolButton * m_staticButt;         /*!< 查看网络统计 */
    QMenu * m_extraMenu;
    QAction * m_autoCreateNetworkAction;
    QAction * m_debugModelAction;
    QToolButton * m_extraButt;          /*!< 额外设置按钮，提供下拉框设置 */
    QLabel * m_statusLabel;             /*!< 网络概要信息显示 */

    int m_iRunning,m_iStopped,m_iError;     /*!< 运行、停止、故障的连接数 */

    PacketDetailWidget * m_packetDetailWidget;     /*!< 包详细信息显示窗口 */
};

} //namespace Core

#endif // NETSTATECONTROLTOOL_H
