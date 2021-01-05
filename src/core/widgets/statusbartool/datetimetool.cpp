#include "datetimetool.h"

#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QMouseEvent>
#include <QSettings>

#include <core/constants.h>
#include <base/util/rutil.h>
#include "../../global.h"
#include "../../file/globalconfigfile.h"

namespace Core{

DatetimeTool::DatetimeTool(QWidget *parent):AbstractStatusTool(Base::Id("SYSTEM.DATETIME"),QString(QStringLiteral("系统时间")),parent)
  ,m_configKey("DateTimeFormat")
{
    m_displayFormat.append("yyyy/M/d");
    m_displayFormat.append("yyyy/MM/dd");
    m_displayFormat.append("yyyy/MM/dd h:m A");
    m_displayFormat.append("yyyy/MM/dd hh:mm");
    m_displayFormat.append("yyyy-MM-dd hh:mm:ss");

    m_currDisplayFormat = Base::RUtil::getGlobalValue(QString(Constant::STATUSBAR_STATUSBAR),m_configKey,m_currDisplayFormat).toString();
    if(!m_displayFormat.contains(m_currDisplayFormat))
        m_currDisplayFormat = m_displayFormat.last();

    generateSingleLayout();
}

DatetimeTool::~DatetimeTool()
{

}

void DatetimeTool::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    static int t_clickIndex = m_displayFormat.indexOf(m_currDisplayFormat);
    m_currDisplayFormat = m_displayFormat.at(++t_clickIndex % m_displayFormat.size());

	Base::RUtil::setGlobalValue(QString(Constant::STATUSBAR_STATUSBAR),m_configKey,m_currDisplayFormat);
}

void DatetimeTool::updateTime()
{
    m_displayLabel->setText(QDateTime::currentDateTime().toString(m_currDisplayFormat));
}

void DatetimeTool::generateSingleLayout()
{
    m_displayLabel = new QLabel;

    m_intervalTimer = new QTimer();
    m_intervalTimer->setInterval(1000);
    connect(m_intervalTimer,SIGNAL(timeout()),this,SLOT(updateTime()));
    m_intervalTimer->start();

    setContentWidget(m_displayLabel);
}

} //namespace Core
