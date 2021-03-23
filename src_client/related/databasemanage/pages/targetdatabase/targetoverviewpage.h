/*!
 * @brief     Ŀ��Ԥ������   
 * @details   �ý���������ʾ���µ�Ŀ����Ϣ�� ����ɾ�Ĳ����
 * @author    yzg
 * @version   1.0
 * @date      2021.01.20 11:11:36
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>
#include <base/selfwidget/rmessagebox.h>
#include <base\util\rutil.h>

#include "../abstractpage.h"
#include "../../customwidget/pageswitchbar.h"
#include "../../customwidget/operationtoolspage.h"

#include "tablemodel/targetoverviewmodel.h"

namespace Related {

	class TargetOverViewPage : public AbstractPage
	{
		Q_OBJECT

	public:
		TargetOverViewPage(QWidget *parent = nullptr);
		~TargetOverViewPage();

		PageType getPageType() const;

		void prepareBringToTop();

	signals:
		void signalSelecteTargetDataInfo(QString indexId);


	private slots:
		void respToolButtPressed(OperationToolsPage::ButtType type);
		void setPageNum(int page);
		void setFixedPageRowCount(int pageItemCount);
		void slotTableDoubleClicked(const QModelIndex & index);

		void processQueryTargetListResponse(const Datastruct::LoadAllTargetResponse & response);
		void processTargetCreateResponse(const Datastruct::TargetCreateResponse & response);
		void processTargetDeleteResponse(const Datastruct::TargetDeleteResponse & response);
		void processTargetModifyResponse(const Datastruct::TargetModifyResponse & response);

	private:
		void init();
		void initConnect();

		void refreshCurrPage();
		void createTargetDataInfo();

	private:
		OperationToolsPage *m_operationToolsPage;
		Base::RTableView * m_tableView;	
		TargetOverViewModel *m_tableModel;
		PageSwitchBar * m_pageSwitch;	

		bool m_firstLoadData;									/*!< ��һ�μ���ҳ����ʾ */
	};

}//namespace Related 
