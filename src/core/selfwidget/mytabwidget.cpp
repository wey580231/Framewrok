#include "mytabwidget.h"

#include <QTabWidget>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QMap>
#include <QPair>
#include <QTabBar>
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QMetaEnum>
#include <QActionGroup>
#include <functional>

#include <base/util/widgetpropsetting.h>
#include <base/util/rutil.h>

namespace Core{

#define MIN_BAR_PAINT_WIDTH 100

class MyTabWidgetPrivate
{
    Q_DECLARE_PUBLIC(MyTabWidget)
private:
    MyTabWidgetPrivate(MyTabWidget * q):q_ptr(q),tabWidget(NULL),mouseMoveable(false){
        init();
    }

    void init();
    void mousePress(QMouseEvent * event);
    void mouseMove(QMouseEvent * event);
    void mouseRelease(QMouseEvent * event);
    void mouseButtonDbClick(QMouseEvent * event);
    void contextMenu(QContextMenuEvent * event);
    void paintEvent();

    void drawLinearGradient(std::function<void(int,QPainterPath&)> drawFunc);

    MyTabWidget * q_ptr;

    QTabWidget * tabWidget;

    typedef QPair<QString,QWidget*> TabPage;
    QList<TabPage> tabPages;
    QStringList hideTabObjNames;               /*!< 已经隐藏的tab页面标识 */

    QMenu * rightPopMenu;
    QMenu * directMenu;
    QMenu * layoutMenu;
    QMap<Widget::WidgetFeature,QAction *> popActions;               /*!< 标题栏弹出按钮 */
    QMap<MyTabWidget::TabPosition,QAction *> directions;            /*!< 方向切换按钮 */
    QMap<MyTabWidget::LayoutDirection,QAction *> layoutDirections;  /*!< 方向切换按钮 */

    bool mouseMoveable;         /*!< 鼠标点下的位置是否支持移动 */
};

void MyTabWidgetPrivate::init()
{
    QWidget * mainWidget = new QWidget;
    mainWidget->setAttribute(Qt::WA_TransparentForMouseEvents,false);
    mainWidget->setObjectName("tabWidget_mainWidget");

    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(1,1,1,1);

    tabWidget = new QTabWidget;
    tabWidget->setTabPosition(QTabWidget::South);

    mainLayout->addWidget(tabWidget);
    mainWidget->setLayout(mainLayout);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(1,1,1,1);
    layout->addWidget(mainWidget);

    q_ptr->setLayout(layout);
    tabWidget->installEventFilter(q_ptr);

    /**********右键弹出菜单************/
    rightPopMenu = new QMenu();
    Base::WidgetPropSetting::enableWidgetTransparency(rightPopMenu,true);

    QAction * action = new QAction(rightPopMenu);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(triggerMoveable(bool)));
    action->setCheckable(true);
    popActions.insert(Widget::WidgetMovable,action);

    action = new QAction(rightPopMenu);
    action->setCheckable(true);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(triggerResizeable(bool)));
    popActions.insert(Widget::WidgetResizeable,action);

    action = new QAction(rightPopMenu);
    action->setCheckable(true);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(triggerExpanable(bool)));
    popActions.insert(Widget::WidgetExpanable,action);

    QList<QAction *> valueList = popActions.values();
    std::for_each(valueList.begin(),valueList.end(),[&](QAction * action){
        rightPopMenu->addAction(action);
    });

    layoutMenu = new QMenu();
    QActionGroup * layGroup = new QActionGroup(layoutMenu);
    QMetaEnum layInfo = QMetaEnum::fromType<MyTabWidget::LayoutDirection>();
    for(int i = 0; i < layInfo.keyCount();i++){
        QAction * dirAction = new QAction();
        layGroup->addAction(dirAction);
        dirAction->setData(layInfo.value(i));
        dirAction->setCheckable(true);
        QObject::connect(dirAction,SIGNAL(triggered(bool)),q_ptr,SLOT(switchLayoutDirect(bool)));
        layoutMenu->addAction(dirAction);
        MyTabWidget::LayoutDirection tabP = static_cast<MyTabWidget::LayoutDirection>(layInfo.value(i));
        layoutDirections.insert(tabP,dirAction);
    }
    rightPopMenu->addMenu(layoutMenu);

    directMenu = new QMenu();
    QActionGroup * actGroup = new QActionGroup(directMenu);
    QMetaEnum enumInfo = QMetaEnum::fromType<MyTabWidget::TabPosition>();
    for(int i = 0; i < enumInfo.keyCount();i++){
        QAction * dirAction = new QAction();
        actGroup->addAction(dirAction);
        dirAction->setData(enumInfo.value(i));
        dirAction->setCheckable(true);
        QObject::connect(dirAction,SIGNAL(triggered(bool)),q_ptr,SLOT(switchDirect(bool)));
        directMenu->addAction(dirAction);
        MyTabWidget::TabPosition tabP = static_cast<MyTabWidget::TabPosition>(enumInfo.value(i));
        directions.insert(tabP,dirAction);
    }
    rightPopMenu->addMenu(directMenu);
}

void MyTabWidgetPrivate::mousePress(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        q_ptr->globalMouseStartPoint = event->globalPos();
        if(q_ptr->testFeatures(Widget::WidgetMovable)){
            mouseMoveable = true;
        }
        q_ptr->setCursor(Qt::ClosedHandCursor);
    }
    q_ptr->raise();
}

void MyTabWidgetPrivate::mouseMove(QMouseEvent *event)
{
    if(mouseMoveable){
        switch (tabWidget->tabPosition()) {
            case QTabWidget::North:q_ptr->countRangeLimitPos(event,q_ptr->South,q_ptr->height() - tabWidget->tabBar()->height());break;
            case QTabWidget::South:q_ptr->countRangeLimitPos(event,q_ptr->North,q_ptr->height() - tabWidget->tabBar()->height());break;
            case QTabWidget::West:q_ptr->countRangeLimitPos(event,q_ptr->East,q_ptr->width() - tabWidget->tabBar()->width());break;
            case QTabWidget::East:q_ptr->countRangeLimitPos(event,q_ptr->West,q_ptr->width() - tabWidget->tabBar()->width());break;
            default: break;
        }
    }
}

void MyTabWidgetPrivate::mouseRelease(QMouseEvent *event)
{
    q_ptr->setCursor(Qt::ArrowCursor);
    mouseMoveable = false;
}

void MyTabWidgetPrivate::mouseButtonDbClick(QMouseEvent *event)
{

}

void MyTabWidgetPrivate::contextMenu(QContextMenuEvent *event)
{
    popActions[Widget::WidgetMovable]->setText(QObject::tr("moveable"));
    popActions[Widget::WidgetResizeable]->setText(QObject::tr("resizeable"));
    popActions[Widget::WidgetExpanable]->setText(QObject::tr("expanded"));

    layoutMenu->setTitle(QObject::tr("layout direction"));
    QMetaEnum layoutInfo = QMetaEnum::fromType<MyTabWidget::LayoutDirection>();
    for(int i = 0; i < layoutInfo.keyCount();i++){
        MyTabWidget::LayoutDirection tabP = static_cast<MyTabWidget::LayoutDirection>(layoutInfo.value(i));
        layoutDirections[tabP]->setText(QObject::tr(layoutInfo.key(i)));
    }

    directMenu->setTitle(QObject::tr("direction"));
    QMetaEnum enumInfo = QMetaEnum::fromType<MyTabWidget::TabPosition>();
    for(int i = 0; i < enumInfo.keyCount();i++){
        MyTabWidget::TabPosition tabP = static_cast<MyTabWidget::TabPosition>(enumInfo.value(i));
        directions[tabP]->setText(QObject::tr(enumInfo.key(i)));
    }

    rightPopMenu->exec(event->globalPos());
}

void MyTabWidgetPrivate::drawLinearGradient(std::function<void(int,QPainterPath&)> drawFunc)
{
    QPainter painter(tabWidget);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor yellowColor(255,195,0,245);
    for(int i = 0; i < 7; i++){
        int marginLeft = 15 * i;
        QPainterPath polygonPath;
        QColor tmpYellowBrush = yellowColor;
        tmpYellowBrush.setAlpha(tmpYellowBrush.alpha() - 13 * i);
        QBrush gradientBrsuh(tmpYellowBrush);
        painter.save();
        drawFunc(marginLeft,polygonPath);
        painter.fillPath(polygonPath,gradientBrsuh);
        painter.restore();
    }
}

void MyTabWidgetPrivate::paintEvent()
{
    QRect barRect = tabWidget->tabBar()->geometry();
    QRect tabRect = tabWidget->geometry();
    QTabWidget::TabPosition direct = tabWidget->tabPosition();

    QColor t_barBackgroundColor(13,26,42,140);

    switch(direct){
        case QTabWidget::North:
            {
                if(tabRect.width() - barRect.width() <= MIN_BAR_PAINT_WIDTH)
                    return;

                {
                    QPainter painter(tabWidget);
                    painter.fillRect(QRect(0,0,tabRect.width(),barRect.height()),t_barBackgroundColor);
                }

                drawLinearGradient([&](int marginLeft,QPainterPath& polygonPath){
                    if(tabWidget->layoutDirection() == Qt::LeftToRight){
                        int xpos = tabRect.width() - marginLeft;
                        polygonPath.moveTo(xpos,barRect.y());
                        polygonPath.lineTo(xpos - 20,barRect.height());
                        polygonPath.lineTo(xpos - 30,barRect.height());
                        polygonPath.lineTo(xpos - 10,barRect.y());
                    }else{
                        int xpos = marginLeft;
                        polygonPath.moveTo(xpos,barRect.y());
                        polygonPath.lineTo(xpos + 20,barRect.height());
                        polygonPath.lineTo(xpos + 30,barRect.height());
                        polygonPath.lineTo(xpos + 10,barRect.y());
                    }
                });
            }
            break;

        case QTabWidget::South:
            {
                if(tabRect.width() - barRect.width() <= MIN_BAR_PAINT_WIDTH)
                    return;

                {
                    QPainter painter(tabWidget);
                    painter.fillRect(QRect(0,barRect.y(),tabRect.width(),barRect.height()),t_barBackgroundColor);
                }

                drawLinearGradient([&](int marginLeft,QPainterPath& polygonPath){
                    if(tabWidget->layoutDirection() == Qt::LeftToRight){
                        int xpos = tabRect.width() - marginLeft;
                        polygonPath.moveTo(xpos,barRect.y());
                        polygonPath.lineTo(xpos - 20,tabRect.height());
                        polygonPath.lineTo(xpos - 30,tabRect.height());
                        polygonPath.lineTo(xpos - 10,barRect.y());
                    }else{
                        int xpos = marginLeft;
                        polygonPath.moveTo(xpos,barRect.y());
                        polygonPath.lineTo(xpos + 20,tabRect.height());
                        polygonPath.lineTo(xpos + 30,tabRect.height());
                        polygonPath.lineTo(xpos + 10,barRect.y());
                    }
                });
            }
            break;

        case QTabWidget::West:
            {
                if(tabRect.height() - barRect.height() <= MIN_BAR_PAINT_WIDTH)
                    return;

                {
                    QPainter painter(tabWidget);
                    painter.fillRect(QRect(0,0,barRect.width(),tabRect.height()),t_barBackgroundColor);
                }

                drawLinearGradient([&](int marginLeft,QPainterPath& polygonPath){
                    int ypos = tabRect.height() - marginLeft;
                    polygonPath.moveTo(barRect.x(),ypos);
                    polygonPath.lineTo(barRect.x(),ypos - 10);
                    polygonPath.lineTo(barRect.width(),ypos - 30);
                    polygonPath.lineTo(barRect.width(),ypos - 20);
                });

            }
            break;

        case QTabWidget::East:
            {
                if(tabRect.height() - barRect.height() <= MIN_BAR_PAINT_WIDTH)
                    return;

                {
                    QPainter painter(tabWidget);
                    painter.fillRect(QRect(barRect.x(),0,barRect.width(),tabRect.height()),t_barBackgroundColor);
                }

                drawLinearGradient([&](int marginLeft,QPainterPath& polygonPath){
                    int ypos = tabRect.height() - marginLeft;
                    polygonPath.moveTo(barRect.x(),ypos);
                    polygonPath.lineTo(barRect.x(),ypos - 10);
                    polygonPath.lineTo(barRect.right(),ypos - 30);
                    polygonPath.lineTo(barRect.right(),ypos - 20);
                });
            }
            break;

        default:break;
    }
}

MyTabWidget::MyTabWidget(QWidget *parent):Widget(parent),d_ptr(new MyTabWidgetPrivate(this))
{
    addWidgetFeatures(WidgetRangeLimit);
    addWidgetFeatures(WidgetDialog);
    setAttribute(Qt::WA_TranslucentBackground);
    enableDialogWindowModule(testFeatures(WidgetDialog));

    setObjectName(Base::RUtil::UUID());
}

MyTabWidget::~MyTabWidget()
{

}

int MyTabWidget::addTab(QWidget *widget, const QString &text)
{
    Q_D(MyTabWidget);
    d->tabPages.append(MyTabWidgetPrivate::TabPage(widget->objectName(),widget));
    return d->tabWidget->addTab(widget,text);
}

int MyTabWidget::insertTab(int index, QWidget *widget, const QString & text)
{
    Q_D(MyTabWidget);
    d->tabPages.append(MyTabWidgetPrivate::TabPage(widget->objectName(),widget));
    return d->tabWidget->insertTab(index,widget,text);
}

void MyTabWidget::removeTab(int index)
{
    Q_D(MyTabWidget);
    d->tabWidget->removeTab(index);
}

/*!
 * @brief 移除指定objName的tab页面
 * @param[in] objName 待移除tab页objectName
 */
bool MyTabWidget::removeTab(QString objName)
{
    Q_D(MyTabWidget);
    int index = 0;
    QList<MyTabWidgetPrivate::TabPage>::iterator iter = d->tabPages.begin();
    while(iter != d->tabPages.end()){
        MyTabWidgetPrivate::TabPage curPage = *iter;
        if(curPage.first == objName){
            d->tabWidget->removeTab(index);
            d->tabPages.erase(iter);
            return true;
        }
        index++;
        iter++;
    }

    return false;
}

/*!
 * @brief 隐藏指定Tab页面
 * @param[in]  objName 待隐藏tab标识
 */
void MyTabWidget::hideTab(QString objName)
{
    Q_D(MyTabWidget);
    if(removeTab(objName)){
        d->hideTabObjNames.append(objName);
    }
}

int MyTabWidget::tabsCount()
{
    Q_D(MyTabWidget);
    return d->tabWidget->count();
}

void MyTabWidget::setTabPosition(MyTabWidget::TabPosition pos)
{
    Q_D(MyTabWidget);
    d->tabWidget->setTabPosition(static_cast<QTabWidget::TabPosition>(pos));
}

MyTabWidget::TabPosition MyTabWidget::getTabPosition()
{
    Q_D(MyTabWidget);
    int pos = static_cast<int>(d->tabWidget->tabPosition());
    return (static_cast<TabPosition>(pos));
}

/*!
 * @brief 获取内容区域相对于屏幕的坐标
 * @details 为了控件在组合与解组合时，其位置与尺寸相对于全局坐标系不改变。根据TabPosition的位置，获取content区域相对于全屏真实的尺寸信息
 * @return 内容区域全局坐标
 */
QRect MyTabWidget::contentGeometry()
{
    Q_D(MyTabWidget);
    QRect newRect;
    QRect tabBarGeometry = d->tabWidget->tabBar()->geometry();
    switch(d->tabWidget->tabPosition()){
        case QTabWidget::North:
            newRect.setTopLeft(QPoint(geometry().x(),geometry().y() + tabBarGeometry.height()));
            newRect.setSize(QSize(geometry().width(),geometry().height() - tabBarGeometry.height()));
            break;
        case QTabWidget::South:
            newRect.setTopLeft(QPoint(geometry().x(),geometry().y()));
            newRect.setSize(QSize(geometry().width(),geometry().height() - tabBarGeometry.height()));
            break;
        case QTabWidget::West:
            newRect.setTopLeft(QPoint(geometry().x()+tabBarGeometry.width(),geometry().y()));
            newRect.setSize(QSize(geometry().width() - tabBarGeometry.width(),geometry().height()));
            break;
        case QTabWidget::East:
            newRect.setTopLeft(QPoint(geometry().x(),geometry().y()));
            newRect.setSize(QSize(geometry().width()- tabBarGeometry.width(),geometry().height()));
            break;
        default:
            break;
    }

    return newRect;
}

/*!
 * @brief 根据内容尺寸，集合tabbar尺寸，计算新框体的尺寸
 * @attention 调用此方法时，窗口还没有初始化完成，因此此时获取的tabBar的geometry的尺寸为100*30(默认尺寸)。此时没有办法来获取真实的尺寸;
 *            遂将tabBar设置固定高度为30.
 * @attention 最终的尺寸要在屏幕的尺寸内部
 * @param[in] contentRect 内容尺寸
 * @return 返回包裹内容后，最新的尺寸
 */
QRect MyTabWidget::setContentGeomtry(QRect contentRect)
{
    Q_D(MyTabWidget);
    QRect newRect;
    d->tabWidget->tabBar()->setFixedHeight(d->tabWidget->tabBar()->height());
    QRect tabBarGeometry = d->tabWidget->tabBar()->geometry();

    switch(d->tabWidget->tabPosition()){
        case QTabWidget::North:
            newRect.setTopLeft(QPoint(contentRect.x(),contentRect.y() - tabBarGeometry.height()));
            newRect.setSize(QSize(contentRect.width(),contentRect.height() + tabBarGeometry.height()));
            break;
        case QTabWidget::South:
            newRect.setTopLeft(QPoint(contentRect.x(),contentRect.y()));
            newRect.setSize(QSize(contentRect.width(),contentRect.height() + tabBarGeometry.height()));
            break;
        case QTabWidget::West:
            newRect.setTopLeft(QPoint(contentRect.x() - tabBarGeometry.width(),contentRect.y()));
            newRect.setSize(QSize(contentRect.width() + contentRect.width(),contentRect.height()));
            break;
        case QTabWidget::East:
            newRect.setTopLeft(QPoint(contentRect.x(),contentRect.y()));
            newRect.setSize(QSize(contentRect.width()+ tabBarGeometry.width(),contentRect.height()));
            break;
        default:
            break;
    }

    QRect screenSize = Base::RUtil::screenGeometry();
    if(testFeatures(Widget::WidgetRangeLimit) && !screenSize.contains(newRect)){
        QRect interRect = screenSize.intersected(newRect);
        QPoint leftPos = newRect.topLeft();
        QPoint intersectedPos(newRect.width() - interRect.width(),newRect.height() - interRect.height());
        newRect.moveTo(leftPos - intersectedPos);
    }

    return newRect;
}

/*!
 * @brief 设置当前显示的索引
 * @param[in]  index 指定显示的索引
 */
void MyTabWidget::setCurrentIndex(int index)
{
    Q_D(MyTabWidget);
    if(index < 0 || index >= tabsCount())
        index = 0;
    d->tabWidget->setCurrentIndex(index);
}

void MyTabWidget::setCurrentWidget(QWidget *widget)
{
    Q_D(MyTabWidget);
    if(widget == NULL)
        return;
    d->tabWidget->setCurrentWidget(widget);
}

int MyTabWidget::currentIndex()
{
    Q_D(MyTabWidget);
    return d->tabWidget->currentIndex();
}

QWidget *MyTabWidget::currentWidget() const
{
    Q_D(const MyTabWidget);
    return widget(d->tabWidget->currentIndex());
}

QWidget *MyTabWidget::widget(int index) const
{
    Q_D(const MyTabWidget);
    return d->tabWidget->widget(index);
}

/*!
 * @brief 获取所有tab页面的objectName
 * @return objectName集合
 */
QStringList MyTabWidget::tabIds()
{
    Q_D(MyTabWidget);
    QStringList ids;
    QList<MyTabWidgetPrivate::TabPage>::iterator iter = d->tabPages.begin();
    while(iter != d->tabPages.end()){
        ids<<(*iter).first;
        iter++;
    }
    return ids;
}

bool MyTabWidget::eventFilter(QObject *watched, QEvent *event)
{
    Q_D(MyTabWidget);
    if(watched == d->tabWidget){
        switch(event->type()){
            case QEvent::MouseButtonPress:{
                QMouseEvent * eve = dynamic_cast<QMouseEvent *>(event);
                d->mousePress(eve);
                return true;
            }
            case QEvent::MouseMove:{
                QMouseEvent * eve = dynamic_cast<QMouseEvent *>(event);
                d->mouseMove(eve);
                return true;
            }
            case QEvent::MouseButtonRelease:{
                QMouseEvent * eve = dynamic_cast<QMouseEvent *>(event);
                d->mouseRelease(eve);
                return true;
            }
            case QEvent::MouseButtonDblClick:{
                QMouseEvent * eve = dynamic_cast<QMouseEvent *>(event);
                d->mouseButtonDbClick(eve);
                return true;
            }
            case QEvent::ContextMenu:{
                d->contextMenu(dynamic_cast<QContextMenuEvent *>(event));
                return true;
            }
            case QEvent::Paint:{
                d->paintEvent();
                break;
            }
            case QEvent::Resize:{

                    break;
                }
            case QEvent::Enter:{
//                    setCursor(Qt::OpenHandCursor);
                    break;
                }
            case QEvent::Leave:{
                    setCursor(Qt::ArrowCursor);
                    break;
                }
            default:
                break;
        }
    }
    return Widget::eventFilter(watched,event);
}

void MyTabWidget::updateFeatures()
{
    Q_D(MyTabWidget);
    d->popActions[Widget::WidgetResizeable]->setChecked(testFeatures(WidgetResizeable));
    d->popActions[Widget::WidgetMovable]->setChecked(testFeatures(WidgetMovable));
    d->popActions[Widget::WidgetExpanable]->setChecked(widgetExpanded);

    int tabp = static_cast<int>(d->tabWidget->tabPosition());
    d->directions[static_cast<MyTabWidget::TabPosition>(tabp)]->setChecked(true);

    int layp = static_cast<int>(d->tabWidget->layoutDirection());
    d->layoutDirections[static_cast<MyTabWidget::LayoutDirection>(layp)]->setChecked(true);
    enableDialogWindowModule(testFeatures(WidgetDialog));
    update();
}

void MyTabWidget::triggerMoveable(bool moveable)
{
    Q_D(MyTabWidget);
    if(moveable)
        currentFeatures |= Widget::WidgetMovable;
    else
        currentFeatures &= ~Widget::WidgetMovable;
}

void MyTabWidget::triggerResizeable(bool resizeable)
{
    Q_D(MyTabWidget);
    if(resizeable)
        currentFeatures |= Widget::WidgetResizeable;
    else
        currentFeatures &= ~Widget::WidgetResizeable;
}

void MyTabWidget::triggerExpanable(bool /*expanded*/)
{
    Q_D(MyTabWidget);
     widgetExpanded = !widgetExpanded;
     d->popActions[Widget::WidgetExpanable]->setChecked(widgetExpanded);
}

/*!
 * @brief 切换Tab标签中tab方向
 * @details 切换方向后需调整尺寸
 */
void MyTabWidget::switchDirect(bool /*flag*/)
{
    Q_D(MyTabWidget);
    QAction * act = dynamic_cast<QAction *>(QObject::sender());
    if(act){
        d->tabWidget->setTabPosition(static_cast<QTabWidget::TabPosition>(act->data().toInt()));
        switch (d->tabWidget->tabPosition()) {
            case QTabWidget::North:adjustGeometry(pos(),South,height() - d->tabWidget->tabBar()->height());break;
            case QTabWidget::South:adjustGeometry(pos(),North,height() - d->tabWidget->tabBar()->height());break;
            case QTabWidget::West:adjustGeometry(pos(),East,width() - d->tabWidget->tabBar()->width());break;
            case QTabWidget::East:adjustGeometry(pos(),West,width() - d->tabWidget->tabBar()->width());break;
            default: break;
        }
    }
}

/*!
 * @brief 切换整体布局方向
 */
void MyTabWidget::switchLayoutDirect(bool /*flag*/)
{
    Q_D(MyTabWidget);
    QAction * act = dynamic_cast<QAction *>(QObject::sender());
    if(act){
        d->tabWidget->setLayoutDirection(static_cast<Qt::LayoutDirection>(act->data().toInt()));
    }
}

}
