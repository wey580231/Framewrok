#include "globalstatusinfotool.h"

#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QDebug>
#include <QFontMetrics>

#include "../flowcomponent/flowcomponentcontainer.h"
#include "../flowcomponent/globalstatusflowcomponent.h"
#include "../../global.h"

namespace Core{

StatusInfoConsumerThread::StatusInfoConsumerThread(QObject *parent):RTask(parent)
{

}

StatusInfoConsumerThread::~StatusInfoConsumerThread()
{
    wait();
}

void StatusInfoConsumerThread::startMe()
{
    RTask::startMe();
    runningFlag = true;
    start();
}

void StatusInfoConsumerThread::stopMe()
{
    RTask::stopMe();
    runningFlag = false;
    RGlobal::G_StatusInfoQueue.wakeUpConsumer(true);
}

void StatusInfoConsumerThread::run()
{
    while(runningFlag){

        while(runningFlag && RGlobal::G_StatusInfoQueue.size() == 0){
            RGlobal::G_StatusInfoQueue.wait();
        }

        if(runningFlag){
            std::list<Datastruct::SystemStatusInfo> statusList = RGlobal::G_StatusInfoQueue.takeAll();
            emit newStatusInfo(statusList);
        }
    }
}

GlobalStatusInfoTool::GlobalStatusInfoTool(QWidget * parent):AbstractStatusTool(Base::Id("SYSTEM.STATUSINFO"),QString(QStringLiteral("运行状态记录")),parent)
    ,m_configKey("MaxShowNums"),m_flowComponent(nullptr)
{
    initView();

    m_consumer = new StatusInfoConsumerThread();
    connect(m_consumer,SIGNAL(newStatusInfo(std::list<Datastruct::SystemStatusInfo>)),this,SLOT(processStatusInfo(std::list<Datastruct::SystemStatusInfo>)));
    m_consumer->startMe();
}

GlobalStatusInfoTool::~GlobalStatusInfoTool()
{
    m_consumer->stopMe();
}

void GlobalStatusInfoTool::mousePressEvent(QMouseEvent *event)
{
    FlowComponentContainer::instance()->showCompoent(m_flowComponent->id());
}

void GlobalStatusInfoTool::initView()
{
    QWidget * tWidget = new QWidget();

    m_infoLevelButt = new QToolButton();
    m_infoLevelButt->setToolTip(QStringLiteral("信息等级"));
    m_infoLevelButt->setIcon(QIcon(":/icon/resource/icon/start_state.png"));

    m_currInfoDescLabel = new QLabel();
    m_currInfoDescLabel->setFixedWidth(320);

    QHBoxLayout * tlayout = new QHBoxLayout();
    tlayout->setContentsMargins(1,1,1,1);
    tlayout->setSpacing(2);
    tlayout->addWidget(m_infoLevelButt);
    tlayout->addWidget(m_currInfoDescLabel);

    m_flowComponent = new GlobalStatusFlowComponent();
    FlowComponentContainer::instance()->addComponent(m_flowComponent);

    tWidget->setLayout(tlayout);
    setContentWidget(tWidget);
}

/*!
* @brief 处理最新的状态数据信息
* @details 若存在多批数据，只处理最后一包数据，其余数据直接进入detail页面，待用户需要时再查看
* @param[in] result 最新数据信息集
*/
void GlobalStatusInfoTool::processStatusInfo(std::list<Datastruct::SystemStatusInfo> result)
{
    if(result.size() > 0){
        static QFontMetrics t_fontMetrics = m_currInfoDescLabel->fontMetrics();

        const Datastruct::SystemStatusInfo & data = result.back();

        if(data.m_level == Datastruct::Warning){
            m_infoLevelButt->setIcon(QIcon(":/icon/resource/icon/start_state.png"));
        }else if(data.m_level == Datastruct::Error){
            m_infoLevelButt->setIcon(QIcon(":/icon/resource/icon/static_state.png"));
        }
        m_currInfoDescLabel->setText(t_fontMetrics.elidedText(data.m_content,Qt::ElideRight,m_currInfoDescLabel->width()));

        m_flowComponent->processStatusInfos(result);
    }
}

} //namespace Core
