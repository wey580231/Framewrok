/*!
 *  @brief     自定义TabWidget
 *  @details   1.可嵌入MyDockWidget @n
 *             2.自身可移动、双击tabbar区域可伸缩 @n
 *  @author    wey
 *  @version   1.0
 *  @date      2018.12.03
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H

#include <QSharedPointer>
#include "widget.h"

namespace Core{

class MyTabWidgetPrivate;

class MyTabWidget : public Widget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MyTabWidget)
public:
    explicit MyTabWidget(QWidget *parent = 0);
    ~MyTabWidget();

    enum LayoutDirection {
        LeftToRight,
        RightToLeft,
        LayoutDirectionAuto
    };
    Q_ENUM(LayoutDirection)

    int addTab(QWidget *widget, const QString &text);
    int insertTab(int index, QWidget *widget, const QString &text);
    void removeTab(int index);
    bool removeTab(QString objName);
    void hideTab(QString objName);
    int tabsCount();

    void setTabPosition(TabPosition pos);
    TabPosition getTabPosition();

    QRect contentGeometry();
    QRect setContentGeomtry(QRect contentRect);

    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget * widget);
    int currentIndex();

    QWidget * currentWidget()const;
    QWidget * widget(int index)const;

    QStringList tabIds();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

protected:
    virtual void updateFeatures();

private slots:
    void triggerMoveable(bool moveable);
    void triggerResizeable(bool moveable);
    void triggerExpanable(bool moveable);
    void switchDirect(bool flag);
    void switchLayoutDirect(bool flag);

private:
    QSharedPointer<MyTabWidgetPrivate> d_ptr;
};

}

#endif // MYTABWIDGET_H
