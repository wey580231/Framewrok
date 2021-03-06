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
#include <QLineEdit>
#include <QFileSystemModel>
#include <QScrollArea>
#include <QTreeView>

#include <base/selfwidget/dialogproxy.h>
#include <base\selfwidget\tablemode\rtableview.h>
#include <base/selfwidget/iconbutton.h>
#include <base/selfwidget/rtabwidget.h>
#include <base\selfwidget\rmessagebox.h>
#include <base/util/rutil.h>

#include <commondefines/protocol.h>

#include "controlwidget/newtaskinfosetwidget.h"
#include "controlwidget/imagemodel.h"

namespace Related {

	class NewTaskDialog : public Base::DialogProxy
	{
	Q_OBJECT

	public:
		/*!
		 * @brief 任务控制类型
		 */
		enum  TaskEditType {
			Task_New,				/*!< 创建 */
			Task_Modify				/*!< 修改 */
		};

		NewTaskDialog(QString taskId, TaskEditType type, QWidget *parent);
		~NewTaskDialog();

		void setTaskBaseInfo(TaskBaseInfo info);
		void setTaskImages(QList<Datastruct::TaskImageEntityData> list);

	private slots:
		void respOk();
		void respClearFile();
		void openLocalFile();
		void switchViewModel(bool isChecked);
		void processTaskCreateResponse(const Datastruct::TaskCreateResponse & response);
		void processTaskModifyResponse(const Datastruct::TaskModifyResponse & response);

		void processTaskDetectPlatformCreateResponse(const Datastruct::TaskDetectPlatformCreateResponse & response);
		void processTaskDetectPlatformModifyResponse(const Datastruct::TaskDetectPlatformModifyResponse & response);

		void processTaskImageCeateResponse(const Datastruct::TaskImageCreateResponse & response);
		void processTaskImageModifyResponse(const Datastruct::TaskImageModifyResponse & response);

	private:
		void init();
		void initConnect();

		void sendNewTaskBaseInfo();
		void sendModifyTaskBaseInfo();

		void sendNewTaskDetectPlatformInfo();
		void sendModifyTaskDetectPlatformInfo();

		void sendNewTaskImageInfo(Datastruct::TaskImageEntityData info);
		void sendModifyTaskImageInfo();

	private:
		QString m_taskId;								/*!< 任务Id */				
		TaskEditType m_taskEditType;					/*!< 任务编辑类型 */

		Base::RTabWidget * m_tabWidget;

		TaskBaseInfo m_taskBaseInfo;					/*!< 任务基本信息 */
		NewTaskInfoSetWidget * m_newTaskWidget;			/*!< 新建任务设置界面 */

		Base::RTableView * m_tableView;
		ImageModel * m_imageTableModel;
		Base::RIconButton * m_viewModelSwitch;

		QScrollArea * m_cardModel;

		QStackedWidget * m_imageStack;

		Base::RTableView * m_fileTableView;
		ImageModel * m_fileTableModel;

		QLineEdit * m_dataFilePath;						/*!< 数据文件路径 */

		QString m_originalFilePath;						/*!< 原始文件路径：绝对路径 */

		QList<OriginalDataFileParameter *> m_taskDataFilePaths;

	};

}//namespace Related 