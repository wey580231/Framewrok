/*!
 *  @brief     地理位置选点及显示标签
 *  @details   整合地图选点的模式,使得简化操作
 *  @author    wey
 *  @version   1.0
 *  @date      2020.09.04
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef RPOSITIONVIEWLABEL_H
#define RPOSITIONVIEWLABEL_H

#include <QWidget>
#include <QLabel>
#include <QStackedWidget>

#include "../../base_global.h"

#include "rlonlatedit.h"

namespace Base{

class IconButton;

typedef QPair<double,double> LocationPos;

class BASESHARED_EXPORT RPositionViewLabel : public QWidget
{
    Q_OBJECT
public:
    RPositionViewLabel(QWidget * parent = nullptr,bool isShowPosEdit = true);
    ~RPositionViewLabel();

    LocationPos getPosition();

    double getLon() const{ return m_lon;}
    double getLat() const{ return m_lat;}

    void setLonLatText(QString lon, QString lat);

    void exitSelectionModel();

signals:
    void signalPositionChanged(LocationPos position);

public slots:
    void setPosition(LocationPos position, QString positionId = (""));

private slots:
    void switchWorkModel(bool flag);
    void switchInputModel(bool flag);

    void slotOnMapSelectPosition(QString positionId, LocationPos position, QString source);
    void slotLineditValueChange(QString value);

    void updateModelChoose(LocationPos position);

private:
    /*!
     *  @brief 工作模式
     */
    enum WorkModel{
        Model_View,         /*!< 正常显示模式 */
        Model_Choose        /*!< 海图选点模式 */
    };

private:
    void init();
    void initConnect();

    LocationPos getPositonInner();

    void updateModelView(LocationPos position);

private:
    QLabel * m_positionLabel;           /*!< 位置信息显示 */
    IconButton * m_chooseMapButt;       /*!< 海图选点按钮 */

    QStackedWidget * m_widgetContainer;

    bool isShowPosEdit;

    RLonLatEdit * m_lonEdit;
    RLonLatEdit * m_latEdit;

    QString m_windowId;         /*!< 窗口句柄ID */
    QString m_positionId;       /*!< 单次海图标绘的产生的ID */

    double m_lon;
    double m_lat;
};

} //namespace Base

#endif // RPOSITIONVIEWLABEL_H
