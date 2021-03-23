/*!
 * @brief     �����¼������ʾ����
 * @details	  
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 11:06:46
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>
#include <base/selfwidget/rmessagebox.h>
#include <base\util\rutil.h>
#include <commondefines/protocol.h>

#include "../abstractpage.h"
#include "../../customwidget/pageswitchbar.h"
#include "../../customwidget/operationtoolspage.h"

#include "tablemodel/experimentrecordmodel.h"
#include "dialog/experimentrecordeditdialog.h"

#define  EXPERIMENT_RECORD_SELET_MAX_INDEX             -1

namespace Related {

	class ExperimentRecordPage : public AbstractPage
	{
		Q_OBJECT

	public:
		ExperimentRecordPage(QWidget *parent  = nullptr);
		~ExperimentRecordPage();

		PageType getPageType() const;

		void prepareBringToTop();

		void setTaskId(QString taskId);

	private slots:
		void respToolButtPressed(OperationToolsPage::ButtType type);
		void setPageNum(int page);
		void setFixedPageRowCount(int pageItemCount);
		void slotDetectPlatformCurrentIndexChanged(int index);
		
		void processQueryAllExperimentRecordResponse(const Datastruct::LoadAllExperimentRecordsResponse & response);
		void processExperimentRecordDeleteResponse(const Datastruct::ExperimentRecordDeleteResponse & response);
		
		void slotClickedTable(QModelIndex index);

	private:
		void init();
		void initConnent();

		void deleteExperimentRecord(QString id);
		void refreshCurrPage();

	private:
		OperationToolsPage * m_operationToolsPage;			/*!< ��������ҳ�� */
		Base::RTableView * m_tableView;						/*!< ���ݱ�� */	
		ExperimentRecordModel * m_tableModel;				/*!< ����ģ�� */
		PageSwitchBar * m_pageSwitch;						/*!< �л�ҳ */
		QComboBox * m_detectPlatformComboBox;

		bool m_firstLoadData;													/*!< ��һ�μ���ҳ����ʾ */
		QString m_taskId;														/*!< ����Id */
		Datastruct::LoadAllExperimentRecordsResponse m_allExperimentRecords;    /*!< ȫ��������Ϣ */
		int m_seleteTableRow;													/*!< ѡ������ */
	};

} // namespace Related 