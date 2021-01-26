/*!
 * @brief     新建任务界面
 *  @details  设置任务基本信息，并上传文件
 * @author    yzg
 * @version   1.0
 * @date      2021.01.22 18:32:39
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 
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

#include <commondefines/protocol.h>

#include "controlwidget/newtaskinfosetwidget.h"

namespace Related {

	class NewTaskDialog : public Base::DialogProxy
	{
		Q_OBJECT

	public:
		NewTaskDialog(QWidget *parent = nullptr);
		~NewTaskDialog();

	signals:
		void signalCreaateNewTask();

	private slots:
		void slotSeleteFile();
		void slotTreeItemClicked(QModelIndex index);
		void respOk();
		void respCancel();

	private:
		void init();
		bool FindFile(const QString &path);
		Base::TreeNode * createTreeNode(Base::TreeNode * parentNode, QString  pasth);
		void updateModel();
		bool getFileNode(Base::TreeNode *node);

		void sendTaskBaseInfo();
		void sendTaskOriginalDataInfo();

	private:
		TaskBaseInfo m_taskBaseInfo;					/*!< 任务基本信息 */
		NewTaskInfoSetWidget *m_newTaskInfoSetWidget;	/*!< 新建任务设置界面 */

		//文件树
		Base::RTreeView * m_treeView;					
		Base::RTreeModel * m_treeModel;
		Base::TreeNode * m_fileRootNode;				/*! <文件树根节点> */

		QLineEdit * m_fileLineEdit;
		Base::RIconButton * m_fileButt;
		QString m_originalFilePath;						/*!< 原始文件路径：绝对路径 */

		QList<OriginalDataFileParameter *> m_taskDataFilePaths;
	};

}//namespace Related 