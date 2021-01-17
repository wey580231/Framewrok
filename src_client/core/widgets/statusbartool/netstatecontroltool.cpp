#include "netstatecontroltool.h"

#include <QString>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QTimer>

#include <base/selfwidget/rmessagebox.h>
#include "../../global.h"
#include "../../file/globalconfigfile.h"
#include "../../pluginmanager/plugin/pluginmanager.h"
#include "../../pluginmanager/plugin/pluginloader.h"
#include "../mainwindow.h"

namespace Core{

PacketDetailWidget::PacketDetailWidget(QWidget *parent):Widget(parent),m_refreshTimer(nullptr),m_contextMenu(nullptr)
{
    setFocusPolicy(Qt::ClickFocus);
    addWidgetFeatures(WidgetDialog);
    enableDialogWindowModule(testFeatures(WidgetDialog));
    setAttribute(Qt::WA_TranslucentBackground);

    intView();
}

PacketDetailWidget::~PacketDetailWidget()
{

}

void PacketDetailWidget::focusInEvent(QFocusEvent *event)
{
    QWidget::focusInEvent(event);
}

void PacketDetailWidget::focusOutEvent(QFocusEvent *event)
{
    if(event->reason() == Qt::ActiveWindowFocusReason){
        this->hide();
    }
    QWidget::focusOutEvent(event);
}

void PacketDetailWidget::showEvent(QShowEvent *event)
{
    if(m_refreshTimer == nullptr){
        m_refreshTimer = new QTimer();
        m_refreshTimer->setInterval(2000);
        connect(m_refreshTimer,SIGNAL(timeout()),this,SLOT(updateTreeItemData()));
    }
    m_refreshTimer->start();
}

void PacketDetailWidget::hideEvent(QHideEvent *event)
{
    m_refreshTimer->stop();
}

bool PacketDetailWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == m_treeWidget){
        if(event->type() == QEvent::FocusOut){
            QFocusEvent * eve = dynamic_cast<QFocusEvent *>(event);
            if(eve && eve->reason() == Qt::ActiveWindowFocusReason){
                this->hide();
            }
            return true;
        }
    }
    return QWidget::eventFilter(watched,event);
}

void PacketDetailWidget::intView()
{
    m_mainWidget = new QWidget();
    m_mainWidget->installEventFilter(this);
    m_mainWidget->setObjectName("mainWidget");

    m_treeWidget = new QTreeWidget();
    m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_treeWidget->installEventFilter(this);
    QStringList tList;
    tList<<QStringLiteral("名称")<<QStringLiteral("协议类型")<<QStringLiteral("网络包数");
    m_treeWidget->setHeaderLabels(tList);
    m_treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(m_treeWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(respContextMenu(QPoint)));

    QMapIterator<QString,NetCardStaticInfo> iter(RGlobal::G_NetCardStaticMap);
    while(iter.hasNext()){
        iter.next();

        QTreeWidgetItem * rootItem = new QTreeWidgetItem(m_treeWidget);
        rootItem->setText(T_Name,iter.key());

        NetCardStaticInfo t_cardInfo = iter.value();
        std::for_each(t_cardInfo.m_protocolStaticsInfo.begin(),t_cardInfo.m_protocolStaticsInfo.end(),[&](ProtocolDataStaticInfo & info){
            QTreeWidgetItem * childItem = new QTreeWidgetItem(rootItem);
            childItem->setText(T_Name,info.m_name);
            childItem->setText(T_Type,QString::number(info.m_iType));
            childItem->setText(T_Count,QString::number(info.m_ulRecvPackets));
            rootItem->addChild(childItem);

            m_treeItems.insert(QString("%1_%2").arg(iter.key()).arg(info.m_iType),childItem);
        });
        m_treeWidget->addTopLevelItem(rootItem);
    }

    m_treeWidget->expandAll();

    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(3,3,3,2);
    mainLayout->addWidget(m_treeWidget);
    m_mainWidget->setLayout(mainLayout);

    QHBoxLayout * t_layout = new QHBoxLayout();
    t_layout->setContentsMargins(0,0,0,0);
    t_layout->addWidget(m_mainWidget);
    setLayout(t_layout);
}

void PacketDetailWidget::updateTreeItemData()
{
    QMapIterator<QString,NetCardStaticInfo> titer(RGlobal::G_NetCardStaticMap);
    while(titer.hasNext()){
        titer.next();
        std::for_each(titer.value().m_protocolStaticsInfo.begin(),titer.value().m_protocolStaticsInfo.end(),[&](ProtocolDataStaticInfo info){
            QString key = QString("%1_%2").arg(titer.value().m_networkId).arg(info.m_iType);
            if(m_treeItems.contains(key)){
                m_treeItems.value(key)->setText(T_Count,QString::number(info.m_ulRecvPackets));
            }
        });
    }
}

void PacketDetailWidget::respContextMenu(QPoint point)
{
    if(m_contextMenu == nullptr){
        m_contextMenu = new QMenu(m_treeWidget);

        QAction * t_clearTree = new QAction(m_contextMenu);
        t_clearTree->setText(QStringLiteral("重置统计"));
        connect(t_clearTree,SIGNAL(triggered(bool)),this,SLOT(clearNetStaticInfo(bool)));

        m_contextMenu->addAction(t_clearTree);
    }
    m_contextMenu->exec(QCursor::pos());
}

void PacketDetailWidget::clearNetStaticInfo(bool flag)
{
    Q_UNUSED(flag)
    QMapIterator<QString,NetCardStaticInfo> titer(RGlobal::G_NetCardStaticMap);
    while(titer.hasNext()){
        titer.next();

        NetCardStaticInfo & tinfo = const_cast<NetCardStaticInfo &>(titer.value());
        QMapIterator<int,ProtocolDataStaticInfo> iter(tinfo.m_protocolStaticsInfo);
        while(iter.hasNext()){
            iter.next();
            ProtocolDataStaticInfo & info = const_cast<ProtocolDataStaticInfo &>(iter.value());
            info.m_ulRecvPackets = 0;
        }
    }

    updateTreeItemData();
}


NetStateControlTool::NetStateControlTool(QWidget *parent):AbstractStatusTool(Base::Id("SYSTEM.NETSTATECONTROL"),QString(QStringLiteral("网络状态控制")),parent)
  ,m_iRunning(0),m_iStopped(0),m_iError(0),m_packetDetailWidget(nullptr)
{
    generateSingleLayout();
    initConnect();
}

NetStateControlTool::~NetStateControlTool()
{

}

bool NetStateControlTool::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == m_statusLabel){
        if(event->type() == QEvent::Paint){

            QPainter painter(m_statusLabel);
            QRect t_drawRect = m_statusLabel->rect();
            //绘制绿、红两种故障提示信息
            int t_rectSpace = 1;
            int t_drawNums = 3;
            int t_halfWidth = (t_drawRect.width() - t_rectSpace * (t_drawNums - 1) )/ t_drawNums;

            QFont t_font = painter.font();
            t_font.setPixelSize(12);
            t_font.setBold(true);
            painter.setFont(t_font);

            painter.setBrush(Qt::green);
            painter.setPen(Qt::NoPen);
            int t_iRectHeightDiff = 2;
            QRect t_drawingRect = QRect(0,t_iRectHeightDiff,t_halfWidth - t_rectSpace,t_drawRect.height() - t_iRectHeightDiff * 2);

            painter.drawRoundedRect(t_drawingRect,5,5);
            painter.setPen(Qt::black);
            painter.drawText(t_drawingRect,Qt::AlignCenter,QString::number(m_iRunning));

            painter.setBrush(Qt::gray);
            painter.setPen(Qt::NoPen);
            t_drawingRect = QRect(t_halfWidth + t_rectSpace,t_iRectHeightDiff,t_halfWidth ,t_drawRect.height() - t_iRectHeightDiff * 2);
            painter.drawRoundedRect(t_drawingRect,5,5);
            painter.setPen(Qt::black);
            painter.drawText(t_drawingRect,Qt::AlignCenter,QString::number(m_iStopped));

            painter.setBrush(Qt::red);
            painter.setPen(Qt::NoPen);
            t_drawingRect = QRect( (t_halfWidth + t_rectSpace)*2,t_iRectHeightDiff,t_halfWidth,t_drawRect.height() - t_iRectHeightDiff * 2);
            painter.drawRoundedRect(t_drawingRect,5,5);
            painter.setPen(Qt::white);
            painter.drawText(t_drawingRect,Qt::AlignCenter,QString::number(m_iError));

            return true;
        }
    }
    return AbstractStatusTool::eventFilter(watched,event);
}

void NetStateControlTool::generateSingleLayout()
{
    QWidget * tWidget = new QWidget();

    m_startButt = new QToolButton();
    m_startButt->setToolTip(QStringLiteral("开启网络"));
    m_startButt->setIcon(QIcon(":/icon/resource/icon/start_state.png"));

    m_stopButt = new QToolButton();
    m_stopButt->setToolTip(QStringLiteral("关闭网络"));
    m_stopButt->setIcon(QIcon(":/icon/resource/icon/stop_state.png"));

    m_staticButt = new QToolButton();
    m_staticButt->setToolTip(QStringLiteral("网络信息统计"));
    m_staticButt->setIcon(QIcon(":/icon/resource/icon/static_state.png"));

    m_extraButt = new QToolButton();
    m_extraButt->setFixedWidth(15);
    m_extraButt->setPopupMode(QToolButton::InstantPopup);

    m_extraMenu = new QMenu();

    m_autoCreateNetworkAction = new QAction();
    m_autoCreateNetworkAction->setCheckable(true);
    m_autoCreateNetworkAction->setText(QStringLiteral("运行即创建网络"));
    m_autoCreateNetworkAction->setChecked(RGlobal::G_GlobalConfigFile->systemConfigInfo.m_bNetworkAutoCreateInStartProcess);
    connect(m_autoCreateNetworkAction,SIGNAL(triggered(bool)),this,SLOT(respAutoStartNetwork(bool)));

    m_debugModelAction = new QAction();
    m_debugModelAction->setCheckable(true);
    m_debugModelAction->setText(QStringLiteral("开启调试模式"));
    m_debugModelAction->setChecked(RGlobal::G_GlobalConfigFile->systemConfigInfo.m_bDebugNetParsedInfo);
    connect(m_debugModelAction,SIGNAL(triggered(bool)),this,SLOT(respDebugNetwork(bool)));

    m_extraMenu->addAction(m_autoCreateNetworkAction);
    m_extraMenu->addSeparator();
    m_extraMenu->addAction(m_debugModelAction);

    m_extraButt->setMenu(m_extraMenu);

    m_statusLabel = new QLabel();
    m_statusLabel->setFixedWidth(70);
    m_statusLabel->installEventFilter(this);
    updateStatusLabel();

    connect(m_startButt,SIGNAL(pressed()),this,SLOT(respStartNetwork()));
    connect(m_stopButt,SIGNAL(pressed()),this,SLOT(respStopNetwork()));
    connect(m_staticButt,SIGNAL(pressed()),this,SLOT(respStatic()));

    m_startButt->setEnabled(!RGlobal::G_GlobalConfigFile->systemConfigInfo.m_bNetworkAutoCreateInStartProcess);
    m_stopButt->setEnabled(RGlobal::G_GlobalConfigFile->systemConfigInfo.m_bNetworkAutoCreateInStartProcess);

    QHBoxLayout * tLayout = new QHBoxLayout();
    tLayout->setContentsMargins(1,1,1,1);
    tLayout->setSpacing(2);
    tLayout->addWidget(m_startButt);
    tLayout->addWidget(m_stopButt);
    tLayout->addWidget(m_staticButt);
    tLayout->addWidget(m_extraButt);
    tLayout->addWidget(m_statusLabel);
    tWidget->setLayout(tLayout);

    setContentWidget(tWidget);
}

void NetStateControlTool::initConnect()
{
    connect(MainWindow::instance(),SIGNAL(netStateChanged()),this,SLOT(updateNetState()));
}

void NetStateControlTool::updateStatusLabel()
{
    m_statusLabel->setToolTip(QString(QStringLiteral("网络连接状态\n已创建：%1\n未创建：%2\n故   障：%3")).arg(m_iRunning).arg(m_iStopped).arg(m_iError));
}

/*!
* @brief 启动网络数据线程
* @details 启动所有tcp、udp服务端，若激活的网络状态队列为空，则先创建网络，否则控制网络开启
*/
void NetStateControlTool::respStartNetwork()
{
    int result = Base::RMessageBox::information(MainWindow::instance(),QStringLiteral("提示"),QStringLiteral("开启所有网络连接?"), Base::RMessageBox::Yes| Base::RMessageBox::No);
    if(result == Base::RMessageBox::Yes){
        if(RGlobal::G_NetworkState.size() == 0){
            NetworkMap * networkMap = PluginLoader::instance()->getNetworks();
            NetworkMap::iterator niter = networkMap->begin();
            while(niter != networkMap->end()){
                MainWindow::instance()->createNetwork(niter.value());
                niter++;
            }
            MainWindow::instance()->registPluginToNetworkAndParseThread();
        }else{
            QList<Datastruct::NetworkState*> tlist = RGlobal::G_NetworkState.values();
            std::for_each(tlist.begin(),tlist.end(),[&](Datastruct::NetworkState* state){
                if(!state->running)
                    MainWindow::instance()->conrtorlNetworkState(state->netId,true);
            });
        }

        m_startButt->setEnabled(false);
        m_stopButt->setEnabled(true);
    }
}

/*!
* @brief 关闭网络数据线程
* @details 关闭所有tcp、udp服务端，对于已有的数据则关闭数据连接
*/
void NetStateControlTool::respStopNetwork()
{
    int result = Base::RMessageBox::information(MainWindow::instance(),QStringLiteral("提示"),QStringLiteral("关闭所有网络连接?"), Base::RMessageBox::Yes| Base::RMessageBox::No);
    if(result == Base::RMessageBox::Yes){
        NetworkMap * networkMap = PluginLoader::instance()->getNetworks();
        NetworkMap::iterator niter = networkMap->begin();
        while(niter != networkMap->end()){
            MainWindow::instance()->conrtorlNetworkState(niter.value().id,false);
            niter++;
        }

        m_startButt->setEnabled(true);
        m_stopButt->setEnabled(false);
    }
}

/*!
* @brief 查看网络数据分析
* @details 1.查看各个协议接收正确解析、错误解析的数量；
*          2.查看各个网络连接的状态，提供便捷的网络状态查看
*/
void NetStateControlTool::respStatic()
{
    if(m_packetDetailWidget == nullptr){
        m_packetDetailWidget = new PacketDetailWidget(MainWindow::instance());
    }

    if(m_packetDetailWidget->isHidden()){
        QSize t_widgetSize(420,450);
        QPoint t_widgetPos;

        QPoint t_globalPoint = m_staticButt->mapToGlobal(QPoint(0,0));
        if(t_globalPoint.x() - t_widgetSize.width() / 2 <= 0){
            t_widgetPos.setX(t_globalPoint.x());
            t_widgetPos.setY(t_globalPoint.y() - t_widgetSize.height() - 6);
        }else{
            t_widgetPos.setX(t_globalPoint.x() - t_widgetSize.width() / 2);
            t_widgetPos.setY(t_globalPoint.y() - t_widgetSize.height() - 6);
        }
        m_packetDetailWidget->setGeometry(QRect(t_widgetPos,t_widgetSize));
        m_packetDetailWidget->show();
        m_packetDetailWidget->setFocus(Qt::MouseFocusReason);
    }
}

/*!
 *  @brief 更新网络状态统计信息
 */
void NetStateControlTool::updateNetState()
{
    QList<Datastruct::NetworkState*> tlist = RGlobal::G_NetworkState.values();
    m_iRunning = 0;
    m_iStopped = 0;
    m_iError = 0;

    std::for_each(tlist.begin(),tlist.end(),[&](Datastruct::NetworkState* state){
        if(state->running){
            m_iRunning++;
        }else{
            if(state->isError){
                m_iError++;
            }else{
                m_iStopped++;
            }
        }
    });

    updateStatusLabel();

    update();
}

/*!
* @brief 运行时即启动网络
* @details 勾选/取消勾选后，更新本地的ini文件
* @param[in] flag true：程序运行时即启动网络
*/
void NetStateControlTool::respAutoStartNetwork(bool flag)
{
    RGlobal::G_GlobalConfigFile->systemConfigInfo.m_bNetworkAutoCreateInStartProcess = flag;
    RGlobal::G_GlobalConfigFile->saveFile();
}

void NetStateControlTool::respDebugNetwork(bool flag)
{
    RGlobal::G_GlobalConfigFile->systemConfigInfo.m_bDebugNetParsedInfo = flag;
    RGlobal::G_GlobalConfigFile->saveFile();
}

} //namespace Core
