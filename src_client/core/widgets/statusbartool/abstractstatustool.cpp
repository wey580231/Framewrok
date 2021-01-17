#include "abstractstatustool.h"

#include <QHBoxLayout>
#include <QDebug>

namespace Core{

AbstractStatusTool::AbstractStatusTool(Base::Id id, QString actionText, QWidget *parent):Widget(parent),m_mainWidget(nullptr)
{
    m_item.m_id = id;
    m_item.m_text = actionText;
    m_item.m_widgetContainer = this;
}

AbstractStatusTool::~AbstractStatusTool()
{

}

/*!
 *  @brief 设置内容显示面板
 *  @warning 仅需调用一次即可
 */
void AbstractStatusTool::setContentWidget(QWidget *content)
{
    if(m_mainWidget == nullptr){
        m_mainWidget = new QWidget(this);
        QHBoxLayout * tlayout = new QHBoxLayout();
        tlayout->setContentsMargins(0,0,0,0);
        tlayout->addWidget(m_mainWidget);
        this->setLayout(tlayout);
    }

    if(m_mainWidget->layout()){
        QLayout * tlayout = m_mainWidget->layout();
        for(int i = tlayout->count() - 1; i >= 0; i--){
            if(tlayout->itemAt(i)->widget())
                delete tlayout->itemAt(i)->widget();
        }
        delete tlayout;
    }

    QHBoxLayout * t_hboxlayout = new QHBoxLayout();
    t_hboxlayout->setContentsMargins(0,0,0,0);
    t_hboxlayout->addWidget(content);

    m_mainWidget->setLayout(t_hboxlayout);
}

} //namespace Core
