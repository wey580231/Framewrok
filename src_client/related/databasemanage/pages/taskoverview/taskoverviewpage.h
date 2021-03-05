/*!
 * @brief     任务概览页面   
 * @author    wey
 * @version   1.0
 * @date      2021.01.20 09:13:41
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QtWidgets/QScrollArea>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDateTime>
#include <QListView>

#include <base/selfwidget/iconbutton.h>
#include <base/util/rutil.h>
#include <commondefines/protocol.h>

#include "../../net/signaldispatch.h"
#include "../../net/datanetconnector.h"

#include "../abstractpage.h"
#include "../systemmainpage/overviewitem.h "
#include "../../customwidget/customwidgetcontainer.h"

#include "../systemmainpage/newtaskdialog.h"

#include "taskbaseinfopage.h"
#include "testimagesitem.h"
#include "dialog/testimagesdetaildialog.h"

namespace Related {

	class TaskOverViewPage : public AbstractPage
	{
		Q_OBJECT

	public:
		TaskOverViewPage(QWidget *parent = nullptr);
		~TaskOverViewPage();

		PageType getPageType() const;
		void prepareBringToTop();

		void setTaskId(QString taskId);

	private slots:
		void processTaskSimpleResponse(const Datastruct::TaskSimpleResponse & response);
		void processQueryAllTaskImageResponse(const Datastruct::LoadAllTaskImageResponse & response);
		void slotSelectedImagesIndex(int index);
		void slotModiftTaskBaseInfo();

	private:
		void init();
		void initConnect();
		void updateTaskImages();

		void refreshCurrTaskSimple();

	private:
		OverViewItem * m_taskPlatformType;
		OverViewItem * m_platformdataSize;
		OverViewItem * m_advSize;

		Base::RIconButton * m_modifyTaskButt;

		TaskBaseInfoPage   * m_taskBaseInfoPage;			/*!< 任务基本信息 */

		// 试验图片显示区域
		QScrollArea * m_testImagesScrollArea;
		QWidget * m_testImagesWidget;
		QList<TestImagesItem *> m_imagesItems;

		bool m_firstLoadData;								/*!< 第一次加载页面显示 */
		QString m_taskId;									/*!< 任务Id */
		TaskBaseInfo m_taskBaseInfo;
		QList<Datastruct::TaskImageEntityData> m_taskImageInfos;
	};

} //namespace Related 