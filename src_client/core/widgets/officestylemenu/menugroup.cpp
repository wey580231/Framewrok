#include "menugroup.h"

#include <QList>
#include <QHBoxLayout>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QFontMetrics>
#include <QStyleOptionFocusRect>

namespace Core{

extern MenuShowModel G_MenuModel;          //当前菜单显示模式

RMenuButton::RMenuButton(QWidget* parent):QAbstractButton(parent),m_bMouseIn(false)
{

}

QSize RMenuButton::sizeHint() const
{
    QFontMetrics fm(font());
    int textWidth = fm.width(text());
    int realWidth = 0;
    if(minimumWidth() > 0){
        realWidth = minimumWidth();
    }else{
        realWidth = textWidth + 2 * 12;
    }

    return QSize(realWidth,height());
}

void RMenuButton::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);

    if(isChecked()){
        painter.fillRect(e->rect(),m_selectedBackgroundColor);
        painter.setPen(m_selectedColor);
    }else{
        if(m_bMouseIn){
            painter.fillRect(e->rect(),m_hoverBackgroundColor);
            painter.setPen(m_hoverColor);
        }else{
            painter.fillRect(e->rect(),m_normalBackgroundColor);
            painter.setPen(m_normalColor);
        }
    }

    painter.drawText(e->rect(),Qt::AlignCenter,text());
}

void RMenuButton::enterEvent(QEvent* event)
{
    m_bMouseIn = true;
}

void RMenuButton::leaveEvent(QEvent* event)
{
    m_bMouseIn = false;
}

void RMenuButton::mousePressEvent(QMouseEvent* event)
{
    if(G_MenuModel == EMBEDD_MODEL){
        if(isChecked()){
            return;
        }
    }
    QAbstractButton::mousePressEvent(event);
}

void RMenuButton::mouseDoubleClickEvent(QMouseEvent* event)
{
    setChecked(false);
    emit hideFlowContainer();
}

MenuGroup::MenuGroup(QWidget* parent):QWidget(parent)
{

}

/*!
 * @brief 添加自定义按钮项
 * @details 创建菜单按钮,并且将按钮添加至布局
 * @param[in] name 菜单按钮名称
 * @param[in] menuItem 按钮对应的按钮容器
 */
void MenuGroup::addItem(QString name, QWidget* menuItem)
{
    ItemDesc item;
    item.itemName = name;
    item.itemMenu = menuItem;

    RMenuButton * button = new RMenuButton();
    connect(button,SIGNAL(clicked(bool)),this,SLOT(buttChecked(bool)));
    connect(button,SIGNAL(hideFlowContainer()),this,SIGNAL(hideFlowContainer()));
    button->setMinimumWidth(70);

    QColor blueColor(42,87,154,0);
    QColor blueTextColor(42,87,154);
    QColor whiteColor(241,241,241);
    button->m_normalBackgroundColor = blueColor;
    button->m_normalColor = whiteColor;

    button->m_hoverBackgroundColor = QColor(31,69,112,200);
    button->m_hoverColor = whiteColor;

    button->m_selectedBackgroundColor = whiteColor;
    button->m_selectedColor = blueTextColor;

    button->setCheckable(true);
    button->setChecked(false);
    button->setText(name);

    m_items.insert(button,item);

    if(layout() == nullptr){
        QHBoxLayout * hlayout = new QHBoxLayout();
        hlayout->setContentsMargins(0,0,0,0);
        hlayout->setSpacing(2);
        hlayout->addStretch(1);
        setLayout(hlayout);
    }

    QHBoxLayout * hlayout = dynamic_cast<QHBoxLayout *>(layout());
    if(hlayout){
        int insertIndex = 0;

        for(int i = 0; i < hlayout->count(); i++){
            if(hlayout->itemAt(i)->widget())
                insertIndex++;
        }

        hlayout->insertWidget(insertIndex,button);
    }
}

/*!
 * @brief 取消所有按钮选中状态
 */
void MenuGroup::cancelCheckedState()
{
    QList<RMenuButton *> keys = m_items.keys();
    for(RMenuButton * butt : keys){
        butt->setChecked(false);
    }
}

void MenuGroup::buttChecked(bool checked)
{
    RMenuButton * buttSender = dynamic_cast<RMenuButton *>(QObject::sender());

    QList<RMenuButton *> keys = m_items.keys();
    for(RMenuButton * butt : keys){
        if(butt != buttSender)
            butt->setChecked(false);
    }

    emit menuChecked(m_items.value(buttSender).itemMenu,checked);
}

} //namespace Core
