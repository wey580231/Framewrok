#include "flowcomponentcontainer.h"

#include <QStackedWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QFocusEvent>
#include <QToolButton>
#include <QDebug>

#include <base/util/rutil.h>
#include "abstractflowcomponent.h"
#include "../../file/appconfig.h"
#include "../statusbar.h"

namespace Core{

FlowComponentContainer * FlowComponentContainer::m_instance = nullptr;

FlowComponentContainer::FlowComponentContainer(QWidget *parent):Widget(parent)
{
    m_instance = this;

    setFocusPolicy(Qt::ClickFocus);
    addWidgetFeatures(WidgetDialog);
    enableDialogWindowModule(testFeatures(WidgetDialog));
    setAttribute(Qt::WA_TranslucentBackground);

    initView();
    hide();
}

FlowComponentContainer::~FlowComponentContainer()
{

}

FlowComponentContainer *FlowComponentContainer::instance()
{
    return m_instance;
}

/*!
* @brief 显示指定的组件
* @details 组件窗体的位置默认以状态栏为基准，未来高度可由用户扩展
* @param[in] compId 待显示组件的ID
*/
void FlowComponentContainer::showCompoent(QString compId)
{
    if(m_comps.contains(compId)){
        if(isVisible()){
            hide();
        }else{
            AbstractFlowComponent * comp = m_comps.value(compId);
            m_widgetContainer->setCurrentWidget(comp->contentWidget());
            m_toolBarContainer->setCurrentWidget(comp->toolBarWidget());
            m_panelNameLabel->setText(comp->name());

            QSize ssize = Base::RUtil::screenSize();

            QSize tsize;
            tsize.setWidth(ssize.width() / 2);
            tsize.setHeight(300);

            QPoint point;
            point.setX(ssize.width()/2);
            point.setY(StatusBar::instance()->pos().y() - tsize.height());
            setGeometry(QRect(point,tsize));

            show();
        }
    }
}

void FlowComponentContainer::addComponent(AbstractFlowComponent *comp)
{
    if(comp == nullptr || comp->toolBarWidget() == nullptr || comp->contentWidget() == nullptr)
        return;

    m_toolBarContainer->addWidget(comp->toolBarWidget());
    m_widgetContainer->addWidget(comp->contentWidget());

    m_comps.insert(comp->id(),comp);
}

void FlowComponentContainer::focusOutEvent(QFocusEvent *event)
{
    if(event->reason() == Qt::ActiveWindowFocusReason){
//        this->hide();
    }
    return QWidget::focusOutEvent(event);
}

void FlowComponentContainer::initView()
{
    QWidget * t_widget = new QWidget(this);
    t_widget->setObjectName("mainWidget");

    //工具栏
    QWidget * tToolBar = new QWidget();
    tToolBar->setFixedHeight(25);
    QHBoxLayout * tToolLayout = new QHBoxLayout();
    tToolLayout->setContentsMargins(1,1,1,1);
    tToolLayout->setSpacing(2);

    m_panelNameLabel = new QLabel();
    m_panelNameLabel->setAlignment(Qt::AlignCenter);
    m_panelNameLabel->setContentsMargins(5,0,0,0);
    m_panelNameLabel->setFixedWidth(80);

    m_toolBarContainer = new QStackedWidget();

    m_closeButton = new QToolButton();
    m_closeButton->setIcon(QIcon(":/icon/resource/icon/stop_state.png"));
    m_closeButton->setToolTip(QStringLiteral("关闭"));
    connect(m_closeButton,SIGNAL(clicked(bool)),this,SLOT(hide()));

    QWidget * toolWidget = new QWidget(tToolBar);
    toolWidget->setMinimumWidth(50);
    QHBoxLayout * toolLayout = new QHBoxLayout();
    toolLayout->setContentsMargins(0,0,0,0);
    toolLayout->setSpacing(0);
    toolLayout->addStretch(1);
    toolLayout->addWidget(m_closeButton);
    toolWidget->setLayout(toolLayout);

    tToolLayout->addWidget(m_panelNameLabel);
    tToolLayout->addWidget(m_toolBarContainer);
    tToolLayout->addWidget(toolWidget);

    tToolBar->setLayout(tToolLayout);

    //内容区
    m_widgetContainer = new QStackedWidget();

    QVBoxLayout * tmainLayout = new QVBoxLayout();
    tmainLayout->setContentsMargins(1,1,1,1);
    tmainLayout->setSpacing(2);

    tmainLayout->addWidget(tToolBar);
    tmainLayout->addWidget(m_widgetContainer);

    t_widget->setLayout(tmainLayout);

    QHBoxLayout * tlayout = new QHBoxLayout();
    tlayout->setContentsMargins(0,0,0,0);
    tlayout->addWidget(t_widget);
    setLayout(tlayout);
}

} //namespace Core
