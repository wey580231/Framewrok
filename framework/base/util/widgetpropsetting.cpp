#include "widgetpropsetting.h"

#include <QWidget>
#include <QAbstractItemView>
#include <QComboBox>
#include <QMenu>
#include <QCalendarWidget>

namespace Base{

/*!
 * @brief 激活窗口透明显示
 * @param[in]  widget 待设置的窗口
 * @param[in]  flag 设置结果
 */
void WidgetPropSetting::enableWidgetTransparency(QWidget *widget, bool flag)
{
    if(flag)
        widget->setWindowFlags(widget->windowFlags() | Qt::FramelessWindowHint);
    else
        widget->setWindowFlags(widget->windowFlags() & ~Qt::FramelessWindowHint);

    widget->setAttribute(Qt::WA_TranslucentBackground,flag);
}

/*!
 * @brief 开启下拉框背景透明效果
 */
void WidgetPropSetting::enableComboxBackgroundTransparency(QComboBox *box)
{
    if(box){
        enableWidgetTransparency(box->view()->parentWidget(),true);
    }
}

/*!
 * @brief 设置日历窗口的透明样式
 */
void WidgetPropSetting::enableCalendarBackgroundTransparency(QCalendarWidget *calendar)
{
    if(calendar){
        //[1] 设置日历中包含菜单按钮为透明显示
        QList<QMenu *> menus = calendar->findChildren<QMenu*>();
        for(QMenu * menu : menus){
            enableWidgetTransparency(menu,true);
        }
    }
}

}
