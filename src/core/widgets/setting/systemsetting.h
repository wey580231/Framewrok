/*!
 *  @brief     系统设置页面
 *  @details   汇集插件中所有的设置，每种设置以单独的页面显示。可动态的插拔。
 *  @author    wey
 *  @version   1.0
 *  @date      2019.01.12
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef SYSTEMSETTING_H_2019_01_12
#define SYSTEMSETTING_H_2019_01_12

#include <base/selfwidget/dialogproxy.h>
#include <QSharedPointer>
#include <core/core_global.h>

namespace Core {

class AbstractSettingPage;
class SystemSettingPrivate;

class CORESHARED_EXPORT SystemSetting : public Base::DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SystemSetting)
public:
    explicit SystemSetting(QWidget *parent = 0);
    ~SystemSetting();

    QSize sizeHint()const;

    /*!< 添加插件页面 */
    void addPage(AbstractSettingPage * page);

private slots:
    void respOk();
    void respCancel();
    void switchPage(int index);

private:
    QSharedPointer<SystemSettingPrivate> d_ptr;

};

}

#endif // SYSTEMSETTING_H
