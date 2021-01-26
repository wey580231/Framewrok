/*!
 * @brief     用户权限管理窗口  
 * @author    wey
 * @version   1.0
 * @date      2021.01.25 14:50:09
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>

#include <base/selfwidget/dialogproxy.h>

namespace Ui { class UserPrivilegeEditWindow; };

namespace Related {

	class UserPrivilegeEditWindow : public Base::DialogProxy
	{
		Q_OBJECT

	public:
		UserPrivilegeEditWindow(QWidget *parent = Q_NULLPTR);
		~UserPrivilegeEditWindow();

		void setInitUserPrivilege(int privilage,bool isManage);

		int getUserPrivilege();
		bool getUserManage();

	private:
		Ui::UserPrivilegeEditWindow *ui;
	};

}	//namespace Related 

