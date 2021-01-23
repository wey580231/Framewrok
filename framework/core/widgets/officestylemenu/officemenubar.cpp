#include "officemenubar.h"

#include <QList>
#include <QHBoxLayout>
#include <QFocusEvent>
#include <QDebug>


#include "abstractofficemenuitem.h"
#include "../officestylewindow.h"

namespace Core{

#define FIXED_MENUBAR_HEIGHT 35
#define FIXED_CONTAINER_HEIGHT 100

MenuShowModel G_MenuModel;          //当前菜单显示模式

OfficeMenuBar::OfficeMenuBar(QWidget* parent):QWidget(parent)
{
    G_MenuModel = FLOW_MODEL;
    init();
}

OfficeMenuBar::~OfficeMenuBar()
{

}

void OfficeMenuBar::addMenu(AbstractOfficeMenuItem* menu)
{
    if(menu == nullptr || menu->menuContainer() == nullptr)
        return;

    m_menuGroup->addItem(menu->menuName(),menu->menuContainer());
    m_menuContainer->addWidget(menu->menuContainer());
}

void OfficeMenuBar::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event)
    if(G_MenuModel == FLOW_MODEL){
        m_menuContainer->hide();
        m_menuGroup->cancelCheckedState();
    }
}

bool OfficeMenuBar::event(QEvent *event)
{
    return QWidget::event(event);
}

void OfficeMenuBar::init()
{
    m_menuGroup = new MenuGroup();
    connect(m_menuGroup,SIGNAL(menuChecked(QWidget*,bool)),this,SLOT(switchMenu(QWidget*,bool)));
    connect(m_menuGroup,SIGNAL(hideFlowContainer()),this,SLOT(hideFlowContainer()));

    QHBoxLayout * hlayout = new QHBoxLayout();
    hlayout->setContentsMargins(0,0,0,0);
    hlayout->addWidget(m_menuGroup);

    m_menuBar = new QWidget();
    m_menuBar->setObjectName("mainWidget");
    m_menuBar->setFixedHeight(FIXED_MENUBAR_HEIGHT);
    m_menuBar->setLayout(hlayout);

    m_menuContainer = new FlowMenuContainer();
    m_menuContainer->setFixedHeight(FIXED_CONTAINER_HEIGHT);
    connect(m_menuContainer,SIGNAL(switchState()),this,SLOT(switchMenuState()));
    connect(m_menuContainer,SIGNAL(cancelButtonCheckState()),this,SLOT(cancelButtonCheckState()));
    m_menuContainer->hide();

    QVBoxLayout * vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0,0,0,0);

    vlayout->addWidget(m_menuBar);
    vlayout->addWidget(m_menuContainer);

    setLayout(vlayout);
}

/*!
 * @brief 切换菜单状态
 */
void OfficeMenuBar::switchToState(MenuShowModel state)
{
    QVBoxLayout * vlayout = dynamic_cast<QVBoxLayout *>(layout());

    //此赋值必须放在对布局操作之前,否则会触发ResizeEvent,导致窗口被隐藏
    G_MenuModel = state;

    if(state == EMBEDD_MODEL){
        m_menuContainer->setParent(this);
        vlayout->addWidget(m_menuContainer);

        animateMenuContainer(true);
        setFixedHeight(FIXED_MENUBAR_HEIGHT + FIXED_CONTAINER_HEIGHT);
    }else if(state == FLOW_MODEL){
        m_menuContainer->hide();
        vlayout->removeWidget(m_menuContainer);

        setFixedHeight(FIXED_MENUBAR_HEIGHT);
    }
}

void OfficeMenuBar::animateMenuContainer(bool visible)
{
    QRect menuBarPos = m_menuBar->rect();

    if(visible){
        m_animation.addVariantAnimation(0,menuBarPos.height(),[&](QVariant value){
            m_menuContainer->move(0,value.toInt());
            this->update();
        });
    }else{
        m_animation.addVariantAnimation(menuBarPos.height(),0,[&](QVariant value){
            m_menuContainer->move(0,value.toInt());
            this->update();
        },300,[&](){
            m_menuContainer->hide();
        });
    }
}

/*!
 * @brief 点击菜单切换菜单项
 * @param[in] widget 将菜单项切换至本窗口
 * @param[in] isChecked true:当前菜单被选中显示,false:在悬浮菜单状态下,隐藏菜单显示
 */
void OfficeMenuBar::switchMenu(QWidget* widget,bool isChecked)
{
    if(widget){
        m_menuContainer->setCurrentWidget(widget);

        if(G_MenuModel == FLOW_MODEL){

            QRect windowRect = OfficeStyleWindow::instance()->geometry();

            if(isChecked){
                if(!m_menuContainer->isVisible()){
                    m_menuContainer->setParent(OfficeStyleWindow::instance());

                    m_menuContainer->setGeometry(0,0,windowRect.width(),FIXED_CONTAINER_HEIGHT);

                    animateMenuContainer(true);

                    m_menuContainer->raise();
                    m_menuContainer->setFocusPolicy(Qt::ClickFocus);
                    m_menuContainer->setFocus(Qt::ActiveWindowFocusReason);
                    Qt::WindowFlags flags = m_menuContainer->windowFlags();
                    m_menuContainer->setWindowFlags(flags | Qt::WindowStaysOnTopHint); // |Qt::Dialog | Qt::FramelessWindowHint
                    m_menuContainer->show();
                }
            }else{
                m_menuContainer->hide();
            }
        }
    }
}

/*!
 * @brief 响应Container中切换按钮操作
 * @details 若从固定切换至隐藏,则需要将按钮选中的状态置为非选中状态
 */
void OfficeMenuBar::switchMenuState()
{
    if(G_MenuModel == FLOW_MODEL){
        switchToState(EMBEDD_MODEL);
    }else if(G_MenuModel == EMBEDD_MODEL){
        hideFlowContainer();
    }
}

/*!
 * @brief 响应双击菜单按钮,隐藏菜单容器
 */
void OfficeMenuBar::hideFlowContainer()
{
    switchToState(FLOW_MODEL);
    m_menuGroup->cancelCheckedState();
    m_menuContainer->cancelCheckedState();
}

/*!
 * @brief 取消切换按钮选中的状态
 * @details 当焦点离开悬浮菜单后，因浮动菜单自动隐藏，需要将对应的按钮取消选中
 */
void OfficeMenuBar::cancelButtonCheckState()
{
    m_menuGroup->cancelCheckedState();
}

} //namespace Core
