#include "rlonlatedit.h"

#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QToolButton>
#include <QAction>
#include <QMenu>

#include "../../util/scaleswitcher.h"
#include "../../util/widgetpropsetting.h"

namespace Base{

RIconEdit::RIconEdit(QWidget *parent):QLineEdit(parent)
{
    init();
}

QAction * RIconEdit::addAction(QString text)
{
    QAction * action = new QAction();
    action->setText(text);

    QMenu * tmenu = button->menu();

    tmenu->addAction(action);

    if(tmenu->actions().size() > 1){
        button->setText(tmenu->actions().at(0)->text());
    }

    return action;
}

void RIconEdit::setToolButtonText(QString text)
{
    button->setText(text);
}

QString RIconEdit::getToolButtonText() const
{
    return button->text();
}

void RIconEdit::init()
{
    button = new QToolButton();
    button->setArrowType(Qt::NoArrow);
    button->setAttribute(Qt::WA_StyledBackground,false);
    button->setPopupMode(QToolButton::InstantPopup);
    button->setStyleSheet("border: none;border-radius: 2px;background-color: rgba(18,34,55,75);color:white");
    button->setFixedSize(22,22);

    button->setMenu(new QMenu());

    //开启透明背景
    WidgetPropSetting::enableWidgetTransparency(button->menu(),true);

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setContentsMargins(1,1,1,0);
    layout->addStretch(1);
    layout->addWidget(button);
    setLayout(layout);

    setTextMargins(0,0,button->width(),0);
}

/************************RLonLatEdit**************************************/


RLonLatEdit::RLonLatEdit(bool isLon, QWidget *parent):RIconEdit(parent),
    m_bLon(isLon)
{
    init();

    m_lonlatDisplayFormat = Display_Degree;
    switchLonLatEditFormat();

    reconnectSignal();
}

void RLonLatEdit::setValue(double val)
{
    disconnectSignal();

    if(m_bLon)
        setToolButtonText(val > 0 ?"E":"W");
    else
        setToolButtonText(val > 0 ?"N":"S");

    if(m_lonlatDisplayFormat == Display_Degree){
        setText(ScaleSwitcher::switchLonLatToDegree(fabs(val)));
    }else if(m_lonlatDisplayFormat == Display_Decimal){
        setText(QString::number(fabs(val),'f',6));
    }

    reconnectSignal();
}

double RLonLatEdit::getValue()
{
    double val = 0;
    if(m_lonlatDisplayFormat == Display_Degree){
        val = ScaleSwitcher::switchDegreeToLonLat(text());
    }else if(m_lonlatDisplayFormat == Display_Decimal){
        val = text().toDouble();
    }

    if(m_bLon){
        val = getToolButtonText().toLower().contains("e")?val:-val;
    }else{
        val = getToolButtonText().toLower().contains("n")?val:-val;
    }

    return val;
}

void RLonLatEdit::switchInputModel(bool flag)
{
    m_decimalAction->setChecked(flag);
    m_lonlatDisplayFormat = (m_lonlatDisplayFormat == Display_Degree)?Display_Decimal:Display_Degree;
    switchLonLatEditFormat();
}

void RLonLatEdit::switchLonLat(bool flag)
{
    QAction * action = dynamic_cast<QAction *>(QObject::sender());
    setToolButtonText(action->text());

    emit inputChanged("");
}

void RLonLatEdit::inputTextChanged(QString inputText)
{
    emit inputChanged(inputText);
}

void RLonLatEdit::init()
{
    if(m_bLon){
        connect(addAction("E"),SIGNAL(triggered(bool)),this,SLOT(switchLonLat(bool)));
        connect(addAction("W"),SIGNAL(triggered(bool)),this,SLOT(switchLonLat(bool)));
    }else{
        connect(addAction("N"),SIGNAL(triggered(bool)),this,SLOT(switchLonLat(bool)));
        connect(addAction("S"),SIGNAL(triggered(bool)),this,SLOT(switchLonLat(bool)));
    }

    m_decimalAction = addAction(QStringLiteral("十进制"));
    m_decimalAction->setCheckable(true);
    connect(m_decimalAction,SIGNAL(triggered(bool)),this,SIGNAL(displayFormatChanged(bool)));
}

void RLonLatEdit::switchLonLatEditFormat()
{
    disconnectSignal();

    if(m_lonlatDisplayFormat == Display_Degree){
        double value = text().toDouble();

        if(m_bLon)
            setInputMask(QStringLiteral("999°99′99″"));
        else
            setInputMask(QStringLiteral(" 99°99′99″"));

        setText(ScaleSwitcher::switchLonLatToDegree(value));
        setValidator(nullptr);
    }else if(m_lonlatDisplayFormat == Display_Decimal){
        QString value = text();

        setInputMask("");

        QDoubleValidator * valid;
        if(m_bLon)
            valid = new QDoubleValidator(-180,180,6,this);
        else
            valid = new QDoubleValidator(-90,90,6,this);

        //WARNING 必须使用这句话,才能起到验证效果
        valid->setNotation(QDoubleValidator::StandardNotation);
        setValidator(valid);

        setText(QString::number(ScaleSwitcher::switchDegreeToLonLat(value),'f',6));
    }

    reconnectSignal();
}

void RLonLatEdit::disconnectSignal()
{
    disconnect(this,SIGNAL(textChanged(QString)),this,SLOT(inputTextChanged(QString)));
}

void RLonLatEdit::reconnectSignal()
{
    connect(this,SIGNAL(textChanged(QString)),this,SLOT(inputTextChanged(QString)));
}

} //namespace Base{
