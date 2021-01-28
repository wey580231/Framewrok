#pragma once

#include <QObject>

#include <commondefines\protocol.h>
#include <commondefines\structdefines.h>
#include <base\util\widgetanimation.h>

namespace Related {

	namespace Global {

		extern Datastruct::UserEntityData G_UserEntity;			/*!< 当前登录的用户 */

		extern bool G_HasPrivilege(Datastruct::UserPrivilege up);	/*!< 查看自身是否具有某种权限 */

		extern Base::WidgetAnimation G_WidgetAnimation;			/*!< 全局控件动画控制 */

	}	//namespace Global {

} //namespace Related 
