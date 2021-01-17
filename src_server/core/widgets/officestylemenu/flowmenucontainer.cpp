#include "flowmenucontainer.h"

#include <QHBoxLayout>
#include <QFocusEvent>
#include <QApplication>
#include <QDebug>
#include <QStyle>

namespace Core{

FlowMenuContainer::FlowMenuContainer(QWidget *parent):QWidget(parent)
{
    init();
}

void FlowMenuContainer::addWidget(QWidget* widget)
{
    m_menuContainer->addWidget(widget);
}

void FlowMenuContainer::setCurrentWidget(QWidget* widget)
{
    m_menuContainer->setCurrentWidget(widget);
}

void FlowMenuContainer::cancelCheckedState()
{
    m_hideButt->setChecked(false);
    switchButtBorderProp(false);
}

void FlowMenuContainer::focusInEvent(QFocusEvent* event)
{
    QWidget::focusInEvent(event);
}

void FlowMenuContainer::focusOutEvent(QFocusEvent* event)
{
    if(!m_hideButt->isChecked() && event->reason() == Qt::ActiveWindowFocusReason){
        this->hide();
        emit cancelButtonCheckState();
    }
    QWidget::focusOutEvent(event);
}


bool FlowMenuContainer::event(QEvent *event)
{
    return QWidget::event(event);
}

void FlowMenuContainer::buttStateChanged(bool flag)
{
    switchButtBorderProp(flag);

    emit switchState();
}

void FlowMenuContainer::init()
{
    m_mainWidget = new QWidget();
    m_mainWidget->setObjectName("mainWidget");

    QHBoxLayout * hlayout = new QHBoxLayout();
    hlayout->setContentsMargins(0,0,0,0);
    hlayout->setSpacing(0);

    m_menuContainer = new QStackedWidget();

    m_hideButt = new QPushButton();
    m_hideButt->setFixedSize(20,20);
    m_hideButt->setCheckable(true);
    buttStateChanged(false);
    connect(m_hideButt,SIGNAL(clicked(bool)),this,SLOT(buttStateChanged(bool)));

    QVBoxLayout * toolLayout = new QVBoxLayout();
    toolLayout->setContentsMargins(0,0,0,0);
    toolLayout->addStretch(1);
    toolLayout->addWidget(m_hideButt);

    hlayout->addWidget(m_menuContainer);
    hlayout->addLayout(toolLayout);

    m_mainWidget->setLayout(hlayout);

    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(m_mainWidget);

    setLayout(mainLayout);
}

void FlowMenuContainer::switchButtBorderProp(bool flag)
{
    if(flag){
        m_hideButt->setStyleSheet("border-image:url(':/icon/resource/icon/folded.png')");
    }else{
        m_hideButt->setStyleSheet("border-image:url(':/icon/resource/icon/fixed.png')");
    }
}

} //namespace Core
