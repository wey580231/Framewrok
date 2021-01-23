/*!
 *  @brief     浮动窗口容器
 *  @details   可容纳多组窗口组件，提供类似于QtCreator中问题等操作面板。可继承AbstractFlowComponent类，增加自定义扩展
 *  @author    wey
 *  @version   1.0
 *  @date      2019.09.12
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef FLOWCOMPONENTCONTAINER_H
#define FLOWCOMPONENTCONTAINER_H

#include "../../selfwidget/widget.h"
#include "../../core_global.h"

class QLabel;
class QStackedWidget;
class QToolButton;

namespace Core{

class AbstractFlowComponent;

class CORESHARED_EXPORT FlowComponentContainer : public Widget
{
    Q_OBJECT
public:
    explicit FlowComponentContainer(QWidget * parent = 0);
    ~FlowComponentContainer();

    static FlowComponentContainer * instance();

    void showCompoent(QString compId);
    void addComponent(AbstractFlowComponent * comp);

protected:
    void focusOutEvent(QFocusEvent * event);

private:
    void initView();

private:
    static FlowComponentContainer * m_instance;

    QStackedWidget * m_toolBarContainer;       /*!< 工具栏显示面板 */
    QStackedWidget * m_widgetContainer;        /*!< 内容显示面板 */
    QToolButton * m_closeButton;

    QLabel * m_panelNameLabel;                 /*!< 当前显示面板信息 */

    QMap<QString,AbstractFlowComponent *> m_comps;       /*!< key:组件的id */
};

} //namespace Core

#endif // FLOWCOMPONENTCONTAINER_H
