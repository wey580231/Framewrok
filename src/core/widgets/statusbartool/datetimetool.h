/*!
 *  @brief     时间工具
 *  @details   提供基本的时间显示，支持设置显示格式(支持保存至config.ini文件)
 *  @author    wey
 *  @version   1.0
 *  @date      2019.08.16
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef DATETIMETOOL_H
#define DATETIMETOOL_H

#include <QList>
#include "abstractstatustool.h"

class QLabel;
class QTimer;

namespace Core{

class DatetimeTool : public AbstractStatusTool
{
    Q_OBJECT
public:
    explicit DatetimeTool(QWidget * parent = 0);
    ~DatetimeTool();

protected:
    void mousePressEvent(QMouseEvent * event);

private slots:
    void updateTime();

private:
    void generateSingleLayout();

private:
    QLabel * m_displayLabel;
    QTimer * m_intervalTimer;
    QList<QString> m_displayFormat;     /*!< 日期格式显示集 */
    QString m_currDisplayFormat;        /*!< 当前显示的格式 */
    QString m_configKey;
};

} //namespace Core

#endif // DATETIMETOOL_H
