#include "globalstatusflowcomponent.h"

#include <QTableView>
#include <QToolButton>
#include <QDebug>
#include <QHBoxLayout>
#include <QString>
#include <QHeaderView>

namespace Core{

StatusModel::StatusModel(QObject *parent):QAbstractTableModel(parent)
{
    headInfo<<QStringLiteral("类型") << QStringLiteral("内容")<< QStringLiteral("信息源")
                  << QStringLiteral("时间");
}

StatusModel::~StatusModel()
{

}

int StatusModel::rowCount(const QModelIndex &parent) const
{
    return m_datalist.size();
}

int StatusModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return headInfo.size();
}

QVariant StatusModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_datalist.size())
        return QVariant();

    int row = index.row();

    switch(role)
    {
        case Qt::TextAlignmentRole:
            return int(Qt::AlignHCenter | Qt::AlignVCenter);
        break;

        case Qt::DisplayRole:
            {
                Datastruct::SystemStatusInfo info = m_datalist.at(index.row());
                switch (static_cast<ColumnType>(index.column())) {
                    case TYPE:
                        return QString("%1").arg(row+1);
                    case CONTENT:
                            return info.m_content;
                        break;
                    case SRC_COMPONENT:
                        return info.m_src;
                    case TIME:
                        return info.m_timestamp;
                    default:
                        return QVariant("");
                }
            }
            break;

        default:
            break;
    }

    return QVariant();
}

QVariant StatusModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (Qt::Horizontal == orientation && section < headInfo.size()){
        return headInfo.at(section);
    }

    return QVariant();
}


GlobalStatusFlowComponent::GlobalStatusFlowComponent(QWidget *parent):AbstractFlowComponent(parent),m_toolBar(nullptr),
    m_content(nullptr)
{

}

GlobalStatusFlowComponent::~GlobalStatusFlowComponent()
{

}

QString GlobalStatusFlowComponent::name()
{
    return QStringLiteral("运行信息");
}

QWidget *GlobalStatusFlowComponent::toolBarWidget()
{
    if(m_toolBar == nullptr){
        m_toolBar = new QWidget();

        m_clearButton = new QToolButton();
        m_clearButton->setToolTip(QStringLiteral("清空"));
        m_clearButton->setIcon(QIcon(":/icon/resource/icon/start_state.png"));

        m_warningButton = new QToolButton();
        m_warningButton->setToolTip(QStringLiteral("显示警告"));
        m_warningButton->setIcon(QIcon(":/icon/resource/icon/start_state.png"));

        m_filterButton = new QToolButton();
        m_filterButton->setToolTip(QStringLiteral("根据分类过滤"));
        m_filterButton->setIcon(QIcon(":/icon/resource/icon/start_state.png"));

        QHBoxLayout * tLayout = new QHBoxLayout();
        tLayout->setContentsMargins(1,1,1,1);
        tLayout->setSpacing(2);
        tLayout->addWidget(m_clearButton);
        tLayout->addWidget(m_warningButton);
        tLayout->addWidget(m_filterButton);
        tLayout->addStretch(1);
        m_toolBar->setLayout(tLayout);
    }
    return m_toolBar;
}

QWidget *GlobalStatusFlowComponent::contentWidget()
{
    if(m_content == nullptr){
        m_content = new QWidget();

        m_tableView = new QTableView();
        m_tableView->horizontalHeader()->setStretchLastSection(true);
        m_tableView->verticalHeader()->setVisible(false);
        m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

        m_statusModel = new StatusModel();

        m_tableView->setModel(m_statusModel);

        QHBoxLayout * tlayout = new QHBoxLayout();
        tlayout->addWidget(m_tableView);
        m_content->setLayout(tlayout);
    }
    return m_content;
}

/*!
* @brief 显示最新的数据记录信息
* @param[in] result 最新一批数据记录
*/
void GlobalStatusFlowComponent::processStatusInfos(std::list<Datastruct::SystemStatusInfo> &result)
{

}

} //namespace Core
