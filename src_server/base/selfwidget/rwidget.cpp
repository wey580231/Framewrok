#include "rwidget.h"

#include <QHBoxLayout>

namespace Base{

RWidget::RWidget(QWidget * parent):QWidget(parent),m_mainWidget(nullptr)
{

}

RWidget::~RWidget()
{

}

void RWidget::setContentWidget(QWidget* childWidget)
{
    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(childWidget);

    m_mainWidget = new QWidget(this);
    m_mainWidget->setObjectName("mainWidget");
    m_mainWidget->setLayout(mainLayout);

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_mainWidget);
    setLayout(layout);
}


} //namespace Base
