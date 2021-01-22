#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QDir>

#include <base/selfwidget/dialogproxy.h>
#include <base/selfwidget/treemode/rtreeview.h>
#include <base/selfwidget/treemode/rtreemodel.h>
#include <base/selfwidget/iconbutton.h>

#include "controlwidget/newtaskinfosetwidget.h"

namespace Related {

	class NewTaskDialog : public Base::DialogProxy
	{
		Q_OBJECT

	public:
		NewTaskDialog(QWidget *parent = nullptr);
		~NewTaskDialog();

	private slots:
		void slotSeleteFile();
		void respOk();
		void respCancel();

	private:
		void init();
		bool FindFile(const QString &path);
		Base::TreeNode * createTreeNode(QString  pasth);
		void updateModel();
		void updateTreeNode();

	private:
		NewTaskInfoSetWidget *m_newTaskInfoSetWidget;

		//ÎÄ¼þÊ÷
		Base::RTreeView *m_treeView;
		Base::RTreeModel *m_treeModel;
		Base::TreeNode *m_fileNode;

		QLineEdit *m_fileLineEdit;
		Base::RIconButton * m_fileButt;
		QString m_originalFilePath;



	};

}//namespace Related 