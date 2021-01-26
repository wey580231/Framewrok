#pragma once

#include <QObject>

#include <commondefines\protocol.h>
#include <commondefines\structdefines.h>

namespace Related {

	namespace Global {

		extern Datastruct::UserEntityData G_UserEntity;			/*!< 当前登录的用户 */

		extern bool G_HasPrivilege(Datastruct::UserPrivilege up);	/*!< 查看自身是否具有某种权限 */

	}	//namespace Global {

} //namespace Related 
