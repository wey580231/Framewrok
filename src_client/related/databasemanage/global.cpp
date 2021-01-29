#include "global.h"

namespace Related {

	namespace Global {

		Datastruct::UserEntityData G_UserEntity;			/*!< 当前登录的用户 */
		LoadingDialog * G_LoadingDialog;					/*!< 等待窗口 */

		Base::WidgetAnimation G_WidgetAnimation;			/*!< 全局控件动画控制 */

		/*! 
		 * @brief 检测当前登录用户是否具有某些权限
		 * @param up 待检测的权限
		 * @return true:具备指定权限;false:不具备指定权限
		 */
		bool G_HasPrivilege(Datastruct::UserPrivilege up) {
			return G_UserEntity.privilege & up;
		}

	} //namespace Global 

} //namespace Related 