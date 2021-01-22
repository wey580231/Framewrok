/*!
 *  @brief     经纬度编辑输入框
 *  @details   支持十进制和度分秒两种格式输入
 *  @author    wey
 *  @version   1.0
 *  @date      2020.09.17
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef RLONLATEDIT_H
#define RLONLATEDIT_H

#include <QLineEdit>

#include "../../base_global.h"

namespace Base{

class BASESHARED_EXPORT RIconEdit : public QLineEdit
{
    Q_OBJECT
public:
    RIconEdit(QWidget * parent = nullptr);
    ~RIconEdit(){}

    QAction *addAction(QString text);

    void setToolButtonText(QString text);
    QString getToolButtonText() const;

private:
    void init();

private:
    QToolButton * button;
};

/*!
 *  @brief 经纬度编辑器
 */
class BASESHARED_EXPORT RLonLatEdit : public RIconEdit
{
    Q_OBJECT
public:
    explicit RLonLatEdit(bool isLon,QWidget * parent = nullptr);
    ~RLonLatEdit(){}

    void setValue(double val);
    double getValue();

signals:
    void inputChanged(QString);

    /*!< 改变显示模式后,1.应捕获此信号,再手动调用switchInputModel,实现切换 */
    void displayFormatChanged(bool flag);

public slots:
    void switchInputModel(bool flag);

private slots:
    void switchLonLat(bool flag);
    void inputTextChanged(QString inputText);

private:
    void init();
    void switchLonLatEditFormat();

    void disconnectSignal();
    void reconnectSignal();

private:
    /*!
     *  @brief 数据显示格式
     */
    enum DisplayFormat{
        Display_Degree,     /*!< 度分秒格式 */
        Display_Decimal     /*!< 十进制格式 */
    };

private:
    bool m_bLon;            /*!< 是否为经度 */

    QAction * m_decimalAction;

    DisplayFormat m_lonlatDisplayFormat;        /*!< 经纬度显示格式 */
};

} //namespace Base

#endif // RLONLATEDIT_H
