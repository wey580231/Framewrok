/*!
 * @brief     
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.03.19 11:30:04
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */ 
#pragma once

#include <QObject>
#include <QMap>

#include <commondefines\protocol.h>
#include <commondefines\structdefines.h>
#include <base\util\widgetanimation.h>
#include <base\tools\notify\notifymanager.h>

#include "customwidget\loadingdialog.h"

/*!< ������������ */
#define START_WAIT Global::G_LoadingDialog->showMe()
#define START_WAIT_ON(parent) Global::G_LoadingDialog->showMe(parent)

#define END_WAIT Global::G_LoadingDialog->hideMe();

/*!< ��ȡ��Դ��Ϣ */
#define WRAP_RESOURCE(name) QStringLiteral(":/QYBlue/resource/qyblue/"#name".png")
#define WRAP_RESOURCE_RADIO(name) QStringLiteral(":/QYBlue/resource/qyblue/radio/"#name".png")

namespace Related {

	namespace Global {

		extern Datastruct::UserEntityData G_UserEntity;			/*!< ��ǰ��¼���û� */
		extern LoadingDialog * G_LoadingDialog;					/*!< �ȴ����� */
		extern Base::NotifyManager * G_Notify;					/*!< ȫ��֪ͨ��Ϣ */

		extern bool G_HasPrivilege(Datastruct::UserPrivilege up);	/*!< �鿴�����Ƿ����ĳ��Ȩ�� */

		extern Base::WidgetAnimation G_WidgetAnimation;			/*!< ȫ�ֿؼ��������� */

	}	//namespace Global 

} //namespace Related 
