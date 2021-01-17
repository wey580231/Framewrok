#include "rpositionviewlabel.h"

#include <QHBoxLayout>
#include <cmath>

#include "../../util/rutil.h"
#include "../../selfwidget/iconbutton.h"

#include "../../util/scaleswitcher.h"
#include "../../util/scaleswitcher.h"

namespace Base{

RPositionViewLabel::RPositionViewLabel(QWidget *parent, bool isShowPosEdit):QWidget(parent),isShowPosEdit(isShowPosEdit),m_lon(0),m_lat(0)
{
    init();
    initConnect();
}

RPositionViewLabel::~RPositionViewLabel()
{

}

void RPositionViewLabel::init()
{
    //正常显示模式
    m_positionLabel = new QLabel();
    m_positionLabel->setAlignment(Qt::AlignCenter);

    //选点模式
    m_lonEdit = new RLonLatEdit(true,this);
    connect(m_lonEdit,SIGNAL(inputChanged(QString)),this,SLOT(slotLineditValueChange(QString)));

    m_latEdit = new RLonLatEdit(false,this);
    connect(m_latEdit,SIGNAL(inputChanged(QString)),this,SLOT(slotLineditValueChange(QString)));

    connect(m_lonEdit,SIGNAL(displayFormatChanged(bool)),this,SLOT(switchInputModel(bool)));
    connect(m_latEdit,SIGNAL(displayFormatChanged(bool)),this,SLOT(switchInputModel(bool)));

    QHBoxLayout * chooseLayout = new QHBoxLayout();
    chooseLayout->setContentsMargins(0,0,0,0);
    chooseLayout->setSpacing(3);
    chooseLayout->addWidget(m_lonEdit);
    chooseLayout->addWidget(m_latEdit);

    QWidget * chooseWidget = new QWidget();
    chooseWidget->setLayout(chooseLayout);

    m_widgetContainer = new QStackedWidget();
    m_widgetContainer->addWidget(m_positionLabel);
    m_widgetContainer->addWidget(chooseWidget);

    m_chooseMapButt = new RIconButton();
    connect(m_chooseMapButt,SIGNAL(toggled(bool)),this,SLOT(switchWorkModel(bool)));
    m_chooseMapButt->setToolTip(QStringLiteral("海图选点"));
    m_chooseMapButt->setCheckable(true);
    m_chooseMapButt->setText("");

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_widgetContainer);
    layout->addWidget(m_chooseMapButt);

    m_chooseMapButt->setVisible(isShowPosEdit);

    setLayout(layout);

    m_windowId = RUtil::UUID();
}

void RPositionViewLabel::initConnect()
{
}

LocationPos RPositionViewLabel::getPositonInner()
{
    double lon = m_lonEdit->getValue();
    double lat = m_latEdit->getValue();

    LocationPos t_position(lon, lat);

    return t_position;
}

/*!
 * @brief 响应从海图上选点后,更新当前数据显示
 * @param[in] position 海图数据点
 */
void RPositionViewLabel::setPosition(LocationPos position, QString positionId)
{
    m_positionId = positionId;
    updateModelView(position);
    updateModelChoose(position);
}

/*!
 * @brief 更新正常模式下数据显示
 * @param[in] position 位置点
 */
void RPositionViewLabel::updateModelView(LocationPos position)
{
    m_lon = position.first;
    m_lat = position.second;

    setLonLatText(ScaleSwitcher::switchLonLatToDegree(fabs(position.first)),ScaleSwitcher::switchLonLatToDegree(fabs(position.second)));
}

/*!
 * @brief 更新选点模式下海图位置信息
 * @param[in] position 海图位置点
 */
void RPositionViewLabel::updateModelChoose(LocationPos position)
{
    m_lonEdit->setValue(position.first);
    m_latEdit->setValue(position.second);
}

/*!
 * @brief 在正常模式和海图选点模式之间切换
 * @param[in] flag true:选点模式;fale:正常模式
 */
void RPositionViewLabel::switchWorkModel(bool flag)
{
    m_widgetContainer->setCurrentIndex(flag ? 1 : 0);
}

void RPositionViewLabel::switchInputModel(bool flag)
{
    m_lonEdit->switchInputModel(flag);
    m_latEdit->switchInputModel(flag);
}

/*!
 * @brief 响应地图选点反馈
 * @param[in] positionId 当前选点窗口选择最新的一个坐标点的ID,待窗口关闭后时,可通过此ID将选择的点从地图上移除
 * @param[in] position 最新的地图位置
 * @param[in] source 选点的点由哪个窗口响应
 */
void RPositionViewLabel::slotOnMapSelectPosition(QString positionId, LocationPos position, QString source)
{
    if(source == m_windowId){
        m_positionId = positionId;

        //同步更新两个模式下的位置信息
        setPosition(position);

        emit signalPositionChanged(position);
    }
}

/**
 * @brief 响应用户编辑经纬度信息,及时将正确的值通过信号发出
 */
void RPositionViewLabel::slotLineditValueChange(QString value)
{
    Q_UNUSED(value);

    LocationPos pos = getPositonInner();

    updateModelView(pos);

    emit signalPositionChanged(pos);
}

LocationPos RPositionViewLabel::getPosition()
{
    LocationPos pos;
    pos.first = m_lon;
    pos.second = m_lat;

    return pos;
}

void RPositionViewLabel::setLonLatText(QString lon, QString lat)
{
    QString pos = QStringLiteral("%2 %1  %4 %3").arg(ScaleSwitcher::getLonAbbreviate(m_lon)).arg(lon).arg(ScaleSwitcher::getLatAbbreviate(m_lat)).arg(lat);
    m_positionLabel->setText(pos);
}

/*!
 * @brief 退出选择模式
 */
void RPositionViewLabel::exitSelectionModel()
{
    m_chooseMapButt->setChecked(false);
    switchWorkModel(false);
}

} //namespace Base
