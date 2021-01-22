/*!
 *  @brief     全局状态运行信息
 *  @details   位于状态栏，可供全局显示运行状态信息，可设置不同状态信息提示(正常、警告、错误)，默认显示一条，但可以查看历史状态信息
 *             用于可设置最大显示条数(可保存配置)
 *  @author    wey
 *  @version   1.0
 *  @date      2019.08.16
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef GLOBALSTATUSINFOTOOL_H
#define GLOBALSTATUSINFOTOOL_H

#include <QAbstractTableModel>
#include "../../network/rtask.h"
#include "abstractstatustool.h"
#include "../../selfwidget/widget.h"
#include "../../protocol/datastruct.h"

class QToolButton;
class QLabel;

namespace Core{

class GlobalStatusFlowComponent;

class StatusInfoConsumerThread : public RTask
{
    Q_OBJECT
public:
    explicit StatusInfoConsumerThread(QObject * parent = 0);
    ~StatusInfoConsumerThread();

    void startMe();
    void stopMe();

signals:
    void newStatusInfo(std::list<Datastruct::SystemStatusInfo> list);

protected:
    void run();
};

class GlobalStatusInfoTool : public AbstractStatusTool
{
    Q_OBJECT
public:
    GlobalStatusInfoTool(QWidget * parent = 0);
    ~GlobalStatusInfoTool();

protected:
    void mousePressEvent(QMouseEvent * event);

private:
    void initView();

private slots:
    void processStatusInfo(std::list<Datastruct::SystemStatusInfo> result);

private:
    QString m_configKey;

    QToolButton * m_infoLevelButt;      /*!< 当前信息显示等级 */
    QLabel * m_currInfoDescLabel;       /*!< 当前信息显示 */

    GlobalStatusFlowComponent * m_flowComponent;

    StatusInfoConsumerThread * m_consumer;
};

} //namespace Core

#endif // GLOBALSTATUSINFOTOOL_H
