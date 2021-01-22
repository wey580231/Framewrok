#include "rdockwidget.h"

namespace Core{

RDockWidget::RDockWidget(QWidget *parent):QDockWidget(parent)
{
    connect(this,SIGNAL(visibilityChanged(bool)),this,SLOT(updateVisibility(bool)));
}

RDockWidget::~RDockWidget()
{

}

/*!
 * @brief 更新窗体可见属性
 * @details dock在tabbed状态下，直接通过isVisible()返回true。然而需要其返回的是为false，遂增加此属性表明其是否可以被用户看见
 * @param[in] visible true:窗体可见；false：窗体不可见
 */
void RDockWidget::updateVisibility(bool visible)
{
    m_visibility = visible;
}

} //namespace Core
