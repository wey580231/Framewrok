#pragma once

#include <QObject>

#include <commondefines\protocol.h>
#include <commondefines\structdefines.h>
#include <base\util\widgetanimation.h>
#include <base\tools\notify\notifymanager.h>

#include "customwidget\loadingdialog.h"

/*!< 开启动画窗口 */
#define START_WAIT Global::G_LoadingDialog->showMe();
#define START_WAIT_ON(parent) Global::G_LoadingDialog->showMe(parent);

#define END_WAIT Global::G_LoadingDialog->hideMe();

/*!< 获取资源信息 */
#define WRAP_RESOURCE(name) QStringLiteral(":/QYBlue/resource/qyblue/"#name".png")
#define WRAP_RESOURCE_RADIO(name) QStringLiteral(":/QYBlue/resource/qyblue/radio/"#name".png")

namespace Related {

	namespace Global {

		extern Datastruct::UserEntityData G_UserEntity;			/*!< 当前登录的用户 */
		extern LoadingDialog * G_LoadingDialog;					/*!< 等待窗口 */
		extern Base::NotifyManager * G_Notify;					/*!< 全局通知消息 */

		extern bool G_HasPrivilege(Datastruct::UserPrivilege up);	/*!< 查看自身是否具有某种权限 */

		extern Base::WidgetAnimation G_WidgetAnimation;			/*!< 全局控件动画控制 */

	}	//namespace Global 

} //namespace Related 
