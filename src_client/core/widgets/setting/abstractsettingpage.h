/*!
 *  @brief     单个系统设置页面
 *  @details   该类对插件开发，插件的设置页面也可以继承后，实现对应的方法，可被添加至systemsetting中。
 *  @author    wey
 *  @version   1.0
 *  @date      2019.01.12
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef ABSTRACTSETTINGPAGE_H
#define ABSTRACTSETTINGPAGE_H

#include <QObject>
#include <core/core_global.h>

class QWidget;

namespace Core {

class CORESHARED_EXPORT AbstractSettingPage : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSettingPage(QObject * parent = 0);
    ~AbstractSettingPage();

    /*!< 设置页面名称 */
    virtual QString pageName() = 0;

    /*!< 设置窗口 */
    virtual QWidget * pageWidget() = 0;

    /*!< 点击ok时操作 */
    virtual bool respOk() = 0;

    /*!< 点击取消时操作 */
    virtual void respCancel() = 0;

};

} // namespace Core

#endif // ABSTRACTSETTINGPAGE_H
