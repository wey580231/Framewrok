/*!
 *  @brief     通用悬浮显示框
 *  @details   提供dock容器显示内容(内容显示区、工具栏区)，由此窗口进行统一的切换。
 *  @author    wey
 *  @version   1.0
 *  @date      2019.09.12
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef ABSTRACTFLOWCOMPONENT_H
#define ABSTRACTFLOWCOMPONENT_H

#include "../../selfwidget/widget.h"
#include "../../core_global.h"

namespace Core{

class CORESHARED_EXPORT AbstractFlowComponent : public Widget
{
    Q_OBJECT
public:
    explicit AbstractFlowComponent(QWidget * parent = 0);
    ~AbstractFlowComponent();

    QString id();
    virtual QString name() = 0;

    virtual QWidget * toolBarWidget() = 0;
    virtual QWidget * contentWidget() = 0;

private:
    QString m_uuid;         /*!< 窗口唯一id，初始化时获取，生存期间不会改变 */
};

} //namespace Core

#endif // ABSTRACTFLOWCOMPONENT_H
