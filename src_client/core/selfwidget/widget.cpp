#include "widget.h"

#include <QMouseEvent>
#include <QDebug>
#include <base/util/rutil.h>
#include "../protocol/datastruct.h"

namespace Core{

Widget::WidgetMap Widget::widgets;

Widget::Widget(QWidget *parent):
    QWidget(parent),currentFeatures(AllWidgetFeatures),widgetExpanded(true),leftButtonPressed(false)
{
    setMouseTracking(true);
}

Widget::~Widget()
{

}

void Widget::setWidgetFeatures(WidgetFeatures feature)
{
    currentFeatures = feature;
    updateFeatures();
}

/*!
 * @brief 添加窗口特性
 * @param[in]  feature 待添加的特性
 */
void Widget::addWidgetFeatures(WidgetFeatures feature)
{
    currentFeatures |= feature;
    updateFeatures();
}

/*!
 * @brief 移除指定的窗口特性
 * @param[in]  feature 待移除的特性
 */
void Widget::removeWidgetFeatures(WidgetFeatures feature)
{
    currentFeatures = currentFeatures & ~feature;
    updateFeatures();
}

Widget::WidgetFeatures Widget::getWidgetFeatures()
{
    return currentFeatures;
}

/*!
 * @brief 处理鼠标点击事件
 * @details 若widget的子类为MyDockWidget，只有不设置WidgetTabbed属性才运行缩放和移动
 * @param[in] event 鼠标事件指针
 */
void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && testFeatures(WidgetResizeable) && !testFeatures(WidgetTabbed)){
        globalMouseStartPoint = event->globalPos();
        leftButtonPressed = true;
        mousePressArea = countMouseArea(event->pos(), countHorizonalArea(event->pos()));
    }
    QWidget::mousePressEvent(event);
}

/*!
 * @details 鼠标移动需要对窗口的属性进行检测,需检测的包括: @a
 *          1.WidgetResizeable
 *          2.WidgetRangeLimit
 */
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(leftButtonPressed && testFeatures(WidgetResizeable))
    {
        int poss = countMouseArea(event->pos(), countHorizonalArea(event->pos()));
        QPoint ptemp = event->globalPos() - globalMouseStartPoint;
        setCursorType(poss);

        if (mousePressArea != 22)
        {
            QRect newGem = geometry();
            switch (mousePressArea)
            {
                case 11:newGem.setTopLeft(newGem.topLeft() + ptemp); break;
                case 12:newGem.setTop(newGem.top() + ptemp.y()); break;
                case 13:newGem.setTopRight(newGem.topRight() + ptemp); break;

                case 21:newGem.setLeft(newGem.left() + ptemp.x()); break;
                case 23:newGem.setRight(newGem.right() + ptemp.x()); break;

                case 32:newGem.setBottom(newGem.bottom() + ptemp.y()); break;
                case 33:newGem.setBottomRight(newGem.bottomRight() + ptemp); break;
                case 31:newGem.setBottomLeft(newGem.bottomLeft() + ptemp); break;

                default:break;
            }

            //NOTE 20190711 修复将窗口缩小至最小尺寸时，无法再次复原问题。遂将判断尺寸的依据改为原来的2倍
            if(newGem.width()<= minimumSize().width() * 2 || newGem.height() <= minimumSize().height() * 2)
                return;

            setGeometry(newGem);
            resizeWindow(true);
        }
         globalMouseStartPoint = event->globalPos();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *)
{
    leftButtonPressed = false;
    resizeWindow(false);
    setCursor(Qt::ArrowCursor);
}

/*!
 * @brief 计算设置了WidgetRangeLimit属性的窗体移动时可移动的位置
 * @param[in] newRect 待移动的位置，用以检测是否超出屏幕显示范围
 * @param[in] direction 屏幕某一方向允许多偏移一定尺寸的距离
 * @param[in] availableHeight 可偏移的距离
 * @return 不超过屏幕显示范围下，允许移动的位置
 */
void Widget::countRangeLimitPos(QMouseEvent * event, TabPosition direction, int availableSize)
{
    QPoint newPos = pos() + (event->globalPos() - globalMouseStartPoint);
    globalMouseStartPoint = event->globalPos();
    adjustGeometry(newPos,direction,availableSize);
}

void Widget::adjustGeometry(QPoint netPosition, TabPosition direction, int availableSize)
{
    if(testFeatures(Widget::WidgetRangeLimit)){
        QRect newRect(netPosition,size());
        QRect screenSize = Base::RUtil::screenGeometry();
        switch(direction){
            case North: screenSize.setY(screenSize.y() - availableSize); break;
            case South: screenSize.setHeight(screenSize.height() + availableSize); break;
            case West: screenSize.setX(screenSize.x() - availableSize); break;
            case East: screenSize.setWidth(screenSize.width() + availableSize); break;
            default:break;
        }
        if(!screenSize.contains(newRect)){
            if(netPosition.x() < screenSize.x())
                netPosition.setX(screenSize.x());
            else if(netPosition.x() > (screenSize.width() + screenSize.x() - newRect.width()))
                netPosition.setX(screenSize.width()+ screenSize.x() - newRect.width());

            if(netPosition.y() < screenSize.y())
                netPosition.setY(screenSize.y());
            else if(netPosition.y() > (screenSize.height() + screenSize.y() - newRect.height()))
                netPosition.setY(screenSize.height()+ screenSize.y() - newRect.height());
        }
    }
    move(netPosition);
}

/*!
 * @brief 设置窗口是否为dialog模式
 * @details 若当前窗口windowFlags包含了Qt::Dialog属性，则窗口由一个frame和titlebar组成；
 *          在设置了Qt::FramelessWindowHint属性后，titlebar消失。
 * @param[in] flag true为设置dialog模式
 */
void Widget::enableDialogWindowModule(bool flag)
{
    Qt::WindowFlags flags = windowFlags();
    if(flag){
        flags |=  Qt::Dialog;
        flags |=  Qt::FramelessWindowHint;
        flags &= ~Qt::Widget;
    }else{
        flags &= ~Qt::Dialog;
        flags &= ~Qt::FramelessWindowHint;
        flags |=  Qt::Widget;
    }
    setWindowFlags(flags);
}

/*!
 * @brief 根据鼠标所属的区域设置鼠标样式
 * @param[in] areaCode 区域代码
 */
void Widget::setCursorType(int areaCode)
{
    Qt::CursorShape cursor;
    switch (areaCode)
    {
        case 11:
        case 33:
            cursor = Qt::SizeFDiagCursor; break;
        case 13:
        case 31:
            cursor = Qt::SizeBDiagCursor; break;
        case 21:
        case 23:
            cursor = Qt::SizeHorCursor; break;
        case 12:
        case 32:
            cursor = Qt::SizeVerCursor; break;
        case 22:
            cursor = Qt::ArrowCursor; break;
        default:
            cursor = Qt::ArrowCursor; break;
            break;
    }
    setCursor(cursor);
}

bool Widget::testFeatures(WidgetFeatures feature)
{
    return this->currentFeatures & feature;
}

void Widget::setObjectName(const QString &name)
{
    QWidget::setObjectName(name);
    if(!widgets.contains(name)){
        widgets.insert(name,this);
    }
}

/*!
 * @brief 返回所有子类对象
 */
Widget::WidgetMap Widget::getAllWidgets()
{
    return widgets;
}

void Widget::setGeometry(int ax, int ay, int aw, int ah)
{
    QWidget::setGeometry(ax,ay,aw,ah);
}

void Widget::setGeometry(const QRect &rect)
{
    QWidget::setGeometry(rect);
}

QRect Widget::getGeometry()const
{
    return geometry();
}

void Widget::setExpanded(bool expandable)
{
    widgetExpanded = expandable;
}

bool Widget::getExpanded()
{
    return widgetExpanded;
}

/*!
 * @brief 计算鼠标水平方向所处位置
 * @attention 20190122:在鼠标点击事件中获取event-pos()，在widget的边上获取宽度是发现，通过event->pos()获取的x值，比widget实际的宽度小1-2像素
 *            为了增加右侧点击区域，加入了2像素偏移量
 * @param[in]  p 鼠标当前的位置
 * @return 区域代码
 */
int Widget::countHorizonalArea(QPoint p)
{
    return (p.x() <= MOUSE_TRIGGER_SPACE) ? 1 : ((width() - p.x() - 2 <= MOUSE_TRIGGER_SPACE) ? 3 : 2);
}

int Widget::countMouseArea(QPoint p, int row)
{
  if (p.y() <= MOUSE_TRIGGER_SPACE)
      return 10 + row;
  else if (p.y() >= height() - MOUSE_TRIGGER_SPACE - 2)
      return 30 + row;
  else
      return 20 + row;
}

}
