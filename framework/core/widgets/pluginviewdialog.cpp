#include "pluginviewdialog.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QDebug>

#include <base/util/rsingleton.h>
#include <core/pluginmanager/plugin/pluginloader.h>
#include <base/selfwidget/iconbutton.h>
#include <base/selfwidget/rmessagebox.h>

namespace Core{

class PluginViewDialogPrivate
{
    Q_DECLARE_PUBLIC(PluginViewDialog)
public:
    PluginViewDialogPrivate(PluginViewDialog * q):q_ptr(q),extandToolWidget(NULL){
        init();
    }

    void init();

    PluginViewDialog * q_ptr;
    QWidget * extandToolWidget;
    QTableView * tableView;
	Base::RIconButton * detailButt;
	Base::RIconButton * errorDetailButt;
};

void PluginViewDialogPrivate::init()
{
    QWidget * mainWidget = new QWidget(q_ptr);

    tableView = new QTableView(mainWidget);
    tableView->setFocusPolicy(Qt::NoFocus);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->verticalHeader()->setVisible(false);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->horizontalHeader()->setStyleSheet("background-color:rgba(0,255,0,50)");
    tableView->horizontalHeader()->setHighlightSections(false);
    tableView->setAlternatingRowColors(true);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    PluginTableModel * model = new PluginTableModel;
    tableView->setModel(model);

    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->addWidget(tableView);
    mainLayout->setContentsMargins(0,0,0,0);
    mainWidget->setLayout(mainLayout);

    extandToolWidget = new QWidget();

    detailButt = new Base::RIconButton();
    QObject::connect(tableView,SIGNAL(clicked(QModelIndex)),q_ptr,SLOT(switchPluginDetail(QModelIndex)));
    QObject::connect(detailButt,SIGNAL(clicked()),q_ptr,SLOT(showPluginDetail()));

    detailButt->setEnabled(false);
    detailButt->setText(QObject::tr("Detail"));

    errorDetailButt = new Base::RIconButton();
    QObject::connect(errorDetailButt,SIGNAL(clicked()),q_ptr,SLOT(showErrorInfo()));
    errorDetailButt->setEnabled(false);
    errorDetailButt->setText(QObject::tr("Error info"));

    QHBoxLayout * extendLayout = new QHBoxLayout;
    extendLayout->setContentsMargins(1,1,1,1);
    extendLayout->addWidget(detailButt);
    extendLayout->addWidget(errorDetailButt);
    extandToolWidget->setLayout(extendLayout);

    q_ptr->setContentWidget(mainWidget);
}

PluginViewDialog::PluginViewDialog(QWidget *parent):Base::DialogProxy(parent),d_ptr(new PluginViewDialogPrivate(this))
{
    setTitle(tr("Loaded plugins"));
    setButton(Base::DialogProxy::Ok,this,SLOT(respOk()));
    setButton(Base::DialogProxy::Close,this,SLOT(respCancel()));

    setExtandToolWidget(d_ptr->extandToolWidget);
}

PluginViewDialog::~PluginViewDialog()
{

}

QSize PluginViewDialog::sizeHint() const
{
    return QSize(550,420);
}

void PluginViewDialog::respOk()
{
    respCancel();
}

void PluginViewDialog::respCancel()
{
    close();
}



PluginTableModel::PluginTableModel(QObject *parent):QAbstractTableModel(parent)
{
    headList<<QStringLiteral("插件名")<<QStringLiteral("插件ID")
           <<QStringLiteral("版本号")<<QStringLiteral("加载状态");
}

PluginTableModel::~PluginTableModel()
{

}

int PluginTableModel::rowCount(const QModelIndex &parent) const
{
    return PluginLoader::instance()->getPluginDesc().size();
}

int PluginTableModel::columnCount(const QModelIndex &parent) const
{
    return T_TOTAL_COUNT;
}

QVariant PluginTableModel::data(const QModelIndex &index, int role)const
{
    QList<PluginDesc> pluginDescList = PluginLoader::instance()->getPluginDesc();
    if(!index.isValid() || index.row() >= pluginDescList.size())
        return QVariant();

    switch(role){
        case Qt::DisplayRole:
                switch(static_cast<TColumn>(index.column())){
                    case T_NAME:return pluginDescList.at(index.row()).pluginName;break;
                    case T_ID: return pluginDescList.at(index.row()).pluginId;break;
                    case T_VERSION:return pluginDescList.at(index.row()).version;break;
                    case T_LOADED:return pluginDescList.at(index.row()).loaded == true ? QStringLiteral("正常") : QStringLiteral("异常");break;
                    default:
                        break;
                }
            break;
        case Qt::TextAlignmentRole: return Qt::AlignCenter; break;
        default:
            break;
    }

    return QVariant();
}

QVariant PluginTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation != Qt::Horizontal || section >= headList.size())
        return QVariant();

    switch(role){
        case Qt::DisplayRole:   return headList.at(section);    break;
        default: return QVariant();
    }
    return QVariant();
}

Qt::ItemFlags PluginTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return flags;
}

void PluginViewDialog::showPluginDetail()
{
    Q_D(PluginViewDialog);
    QModelIndex modelIndex = d->tableView->currentIndex();
    if(modelIndex.isValid()){
        QList<PluginDesc> pluginList = PluginLoader::instance()->getPluginDesc();
        int row = modelIndex.row();
        if(row < pluginList.size()){
            QString content = QString(QStringLiteral("\n[1]插件名字:%1\r\n\r\n[2]插件版本号:%2\r\n\r\n[3]插件ID:%3\r\n\r\n[4]插件路径:%4\r\n\r\n"))
                    .arg(pluginList.at(row).pluginName)
                    .arg(pluginList.at(row).version)
                    .arg(pluginList.at(row).groupId)
                    .arg(pluginList.at(row).location);
			Base::RMessageBox::information(this,tr("Plugin detail"),content, Base::RMessageBox::Yes, Base::RMessageBox::Yes);
        }
    }
}

void PluginViewDialog::switchPluginDetail(QModelIndex activeIndex)
{
    Q_D(PluginViewDialog);
    d->detailButt->setEnabled(false);
    if(activeIndex.isValid()){
        d->detailButt->setEnabled(true);
        PluginDesc selectedDesc = PluginLoader::instance()->getPluginDesc().at(activeIndex.row());
        d->errorDetailButt->setEnabled(!selectedDesc.loaded);
    }
}

/*!
 * @brief 显示加载失败的插件信息
 */
void PluginViewDialog::showErrorInfo()
{
    Q_D(PluginViewDialog);
    QModelIndex modelIndex = d->tableView->currentIndex();
    if(modelIndex.isValid()){
        QList<PluginDesc> pluginList = PluginLoader::instance()->getPluginDesc();
        int row = modelIndex.row();
        if(row < pluginList.size()){
            QString content = QString(tr("[1]error info:%1 \r\n\r\n[2]plugin path:%2")).arg(tr("interface cast error!")).arg(pluginList.at(row).location);
			Base::RMessageBox::warning(this,tr("Error detail"),content, Base::RMessageBox::Yes, Base::RMessageBox::Yes);
        }
    }
}

}
