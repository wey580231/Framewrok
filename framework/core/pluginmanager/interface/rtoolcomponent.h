/*!
 *  @brief     通用工具插件接口
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2019.03.28
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RTOOLCOMPONENT_H
#define RTOOLCOMPONENT_H

#include <QObject>
#include <base/actionmanager/id.h>
#include <core/core_global.h>

namespace Core{

class CORESHARED_EXPORT RToolComponent : public QObject
{
    Q_OBJECT
public:
    explicit RToolComponent(QObject * parent  = 0);

    /*!< 插件生命周期 */
    virtual Base::Id id() = 0;
    virtual bool initialize(QWidget * parent) = 0;
    virtual void release() = 0;
    virtual QString pluginName() = 0;

};

} // namespace Core

#define RComponentInterface_iid "com.rengu.viewframe"
Q_DECLARE_INTERFACE(Core::RToolComponent, RComponentInterface_iid)

#endif // RTOOLCOMPONENT_H
