#include "userprivilegeeditwindow.h"
#include "ui_userprivilegeeditwindow.h"

#include <commondefines\structdefines.h>

namespace Related {

	UserPrivilegeEditWindow::UserPrivilegeEditWindow(QWidget *parent)
		: Base::DialogProxy(parent)
	{
		ui = new Ui::UserPrivilegeEditWindow();
		ui->setupUi(this);

		this->setMinimumHeight(220);
		ui->viewable->setDisabled(true);		//默认有可查看权限

		setTitle(QStringLiteral("用户权限编辑"));
		setButton(DialogProxy::Ok, this, SLOT(accept()));
		setButton(DialogProxy::Cancel, this, SLOT(reject()));

		setContentWidget(ui->widget);
	}

	UserPrivilegeEditWindow::~UserPrivilegeEditWindow()
	{
		delete ui;
	}

	void UserPrivilegeEditWindow::setInitUserPrivilege(int privilage, bool isManage)
	{
		ui->viewable->setChecked(privilage & Datastruct::ReadOnly);
		ui->editable->setChecked(privilage & Datastruct::ReadWrite);
		ui->deleteable->setChecked(privilage & Datastruct::DeleteAble);

		ui->manage->setChecked(isManage);
	}

	int UserPrivilegeEditWindow::getUserPrivilege()
	{
		int privilege = 0;

		if (ui->viewable->isChecked())
			privilege |= Datastruct::ReadOnly;

		if (ui->editable->isChecked())
			privilege |= Datastruct::ReadWrite;

		if (ui->deleteable->isChecked())
			privilege |= Datastruct::DeleteAble;

		return privilege;
	}

	bool UserPrivilegeEditWindow::getUserManage()
	{
		return ui->manage->isChecked();
	}

} //namespace Related 