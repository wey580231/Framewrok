/*!
 *  @brief     通用状态工具类
 *  @details   提供统一调用接口，便于后期转换成插件的扩展需求
 *  @author    wey
 *  @version   1.0
 *  @date      2019.08.16
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef ABSTRACTSTATUSTOOL_H
#define ABSTRACTSTATUSTOOL_H

#include <base/actionmanager/statusbarcontainer.h>
#include "../../selfwidget/widget.h"
#include "../../core_global.h"

namespace Core{

class CORESHARED_EXPORT AbstractStatusTool : public Widget
{
    Q_OBJECT
public:
    explicit AbstractStatusTool(Base::Id id,QString actionText,QWidget * parent = 0);
    ~AbstractStatusTool();

	Base::StatusBarItem getItemInfo(){return m_item;}

protected:
    void setContentWidget(QWidget * content);

protected:
	Base::StatusBarItem m_item;

private:
    QWidget * m_mainWidget;

};

} //namespace Core

#endif // ABSTRACTSTATUSTOOL_H
