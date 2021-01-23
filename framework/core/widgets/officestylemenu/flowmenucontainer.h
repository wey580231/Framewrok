/*!
 *  @brief     自定义菜单项
 *  @details   有两种状态:悬浮和嵌入状态:
 *             1.悬浮状态:由鼠标点击菜单项,触发其显示;
 *             2.嵌入状态:点击功能区按钮后,嵌入至MenuBar布局中
 *  @author    wey
 *  @version   1.0
 *  @date      2020.07.30
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef FLOWMENUCONTAINER_H
#define FLOWMENUCONTAINER_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>

namespace Core{

class FlowMenuContainer : public QWidget
{
    Q_OBJECT
public:
    explicit FlowMenuContainer(QWidget *parent = 0);

    void addWidget(QWidget * widget);
    void setCurrentWidget(QWidget * widget);

    void cancelCheckedState();

signals:
    void switchState();
    void cancelButtonCheckState();

protected:
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);
    bool event(QEvent *event);

private slots:
    void buttStateChanged(bool flag);

private:
    void init();
    void switchButtBorderProp(bool flag);

private:
    QWidget * m_mainWidget;

    QStackedWidget * m_menuContainer;
    QPushButton * m_hideButt;
};

} //namespace Core

#endif // FLOWMENUCONTAINER_H
