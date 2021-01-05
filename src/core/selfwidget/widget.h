/*!
 *  @brief     无边框窗口基类
 *  @details   用于实现通用功能，如边框阴影
 *  @file      widget.h
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.15
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note      此窗口设置了一定的边距，用于绘制渐变的背景。
 *      20180222:LYS:添加获取窗口中阴影边框宽度方法shadowWidth
 *      20180425:wey:添加设置工具栏移动，避免全面板可移动
 *      20180720:LYS:修改enterEvent方法，将阴影边框纳入考虑范围
 */

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>
#include "../core_global.h"

namespace Core{

class WidgetPrivate;

class CORESHARED_EXPORT Widget : public QWidget
{
    Q_OBJECT
    Q_FLAGS(WidgetFeatures)
    Q_DECLARE_PRIVATE(Widget)
public:
    explicit Widget(QWidget * parent);
    ~Widget();

    typedef QMap<QString , Widget * > WidgetMap;

    enum WidgetFeature{
        WidgetClosable    = 0x0001,
        WidgetMovable     = 0x0002,
        WidgetFloatable   = 0x0004,
        WidgetVerticalTitleBar = 0x0008,    /*!< dock窗口中有效 */
        WidgetExpanable = 0x0010,           /*!< 窗口是否展开，titlebar、modulesetting窗口是否隐藏 */
        WidgetResizeable = 0x0020,
        WidgetVisible = 0x0040,
        WidgetRangeLimit = 0x0080,          /*!< 窗口的四周不能出屏幕可视区域 */
        WidgetTabbed = 0x0100,              /*!< dock窗口有效，是否与其它dock嵌入 */
        WidgetDialog = 0x0200,              /*!< 窗口是否为dialog，若为dialog则具有单独的window窗口，拖拽效果好 */

        WidgetFeatureMask = 0xffff,
        AllWidgetFeatures = WidgetClosable|WidgetMovable|WidgetFloatable|WidgetExpanable|WidgetResizeable|WidgetVisible,
        NoWidgetFeatures  = 0x0000,

        Reserved              = 0x00ff
    };
    Q_DECLARE_FLAGS(WidgetFeatures,WidgetFeature)

    enum TabPosition{
        North,
        South,
        West,
        East
    };
    Q_ENUM(TabPosition)

    void setWidgetFeatures(WidgetFeatures feature);
    void addWidgetFeatures(WidgetFeatures feature);
    void removeWidgetFeatures(WidgetFeatures feature);
    Widget::WidgetFeatures getWidgetFeatures();
    bool testFeatures(WidgetFeatures feature);

    void setObjectName(const QString & name);

    static WidgetMap getAllWidgets();
    void setGeometry(int ax, int ay, int aw, int ah);
    virtual void setGeometry(const QRect &rect);
    virtual QRect getGeometry() const;

    void setExpanded(bool expandable);
    bool getExpanded();

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

    void countRangeLimitPos(QMouseEvent *event, TabPosition direction = North, int availableSize = 0);
    void adjustGeometry(QPoint netPosition, TabPosition direction, int availableSize);
    void enableDialogWindowModule(bool flag);

protected:
    virtual void updateFeatures(){}
    virtual void resizeWindow(bool isResizing){Q_UNUSED(isResizing)}

    WidgetFeatures currentFeatures;
    bool widgetExpanded;           /*!< 内容面板是否折叠 */
    QPoint globalMouseStartPoint;       /*!< 全局坐标系下鼠标按下的位置 */
    QPoint localMouseStartPoint;        /*!< 窗口坐标系下鼠标按下的位置 */

private:
    int countMouseArea(QPoint p, int row);
    int countHorizonalArea(QPoint p);
    void setCursorType(int areaCode);

private:
    bool leftButtonPressed;
    int mousePressArea;

    static WidgetMap widgets;
};

}

#endif // ABSTRACTWIDGET_H
