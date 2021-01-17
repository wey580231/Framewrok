/*!
 *  @brief     全局工作状态显示区
 *  @details   提供表格展示当前运行过程中产生的提醒及故障信息
 *  @author    wey
 *  @version   1.0
 *  @date      2019.09.12
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef GLOBALSTATUSFLOWCOMPONENT_H
#define GLOBALSTATUSFLOWCOMPONENT_H

#include <QAbstractTableModel>
#include "abstractflowcomponent.h"
#include "../../protocol/datastruct.h"

class QTableView;
class QToolButton;

namespace Core{

class StatusModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StatusModel(QObject *parent = 0);
    ~StatusModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role) const;

private:
    enum ColumnType
    {
        TYPE,               //告警类型
        CONTENT,            //内容
        SRC_COMPONENT,      //信息源
        TIME                //时间
    };

private:
    QStringList headInfo;

    std::vector<Datastruct::SystemStatusInfo> m_datalist;

};

class GlobalStatusFlowComponent : public AbstractFlowComponent
{
    Q_OBJECT
public:
    explicit GlobalStatusFlowComponent(QWidget * parent = 0);
    ~GlobalStatusFlowComponent();

    QString name() override;
    QWidget * toolBarWidget() override;
    QWidget * contentWidget() override;

    void processStatusInfos(std::list<Datastruct::SystemStatusInfo> & result);

private:
    QWidget * m_toolBar;
    QWidget * m_content;

    QTableView * m_tableView;
    StatusModel * m_statusModel;

    QToolButton * m_clearButton;
    QToolButton * m_warningButton;          /*!< 警告 */
    QToolButton * m_filterButton;           /*!< 提供对信息的过滤 */
};

} //namespace Core

#endif // GLOBALSTATUSFLOWCOMPONENT_H
