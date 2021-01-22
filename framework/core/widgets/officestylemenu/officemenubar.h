/*!
 *  @brief     Office2013风格的菜单
 *  @details   1.支持悬浮和嵌入两种模式; 2.双击菜单或选择固定,则可将悬浮模式的菜单转换成嵌入模式;
 *             3.在固定模式下点击收缩或在悬浮模式下失去光标,则可将菜单区域隐藏
 *  @author    wey
 *  @version   1.0
 *  @date      2020.07.30
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef OFFICEMENUBAR_H
#define OFFICEMENUBAR_H

#include <QWidget>

#include <base/util/widgetanimation.h>
#include "menugroup.h"
#include "flowmenucontainer.h"

namespace Core{

class AbstractOfficeMenuItem;
class OfficeMenuBar;

class OfficeMenuBar : public QWidget
{
    Q_OBJECT
public:
    explicit OfficeMenuBar(QWidget * parent = nullptr);
    ~OfficeMenuBar();

    void addMenu(AbstractOfficeMenuItem * menu);

protected:
    void resizeEvent(QResizeEvent * event);
    bool event(QEvent *event);

private slots:
    void switchMenu(QWidget* widget, bool isChecked);
    void switchMenuState();
    void hideFlowContainer();
    void cancelButtonCheckState();

private:
    void init();
    void switchToState(MenuShowModel state);
    void animateMenuContainer(bool visible);

private:
    QWidget * m_menuBar;                /*!< 菜单栏 */

    MenuGroup * m_menuGroup;            /*!< 菜单栏中的菜单部分 */
    FlowMenuContainer * m_menuContainer;

    Base::WidgetAnimation m_animation;
};

} //namespace Core

#endif // OFFICEMENUBAR_H
