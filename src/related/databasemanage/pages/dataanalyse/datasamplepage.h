/*!
 *  @brief	   数据样本管理界面
 *  @details   1.用于在表格中显示样本原始数；
 *  @author    yzg
 *  @version   1.0
 *  @date      2021.01.13
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>

#include <base\selfwidget\tablemode\rtableview.h>
#include <base\selfwidget\iconbutton.h>

#include "tablemodel/datasamplemodel.h"

#include "gatherwidget.h"
#include "inspectwidget.h"

namespace Related {

	class datasamplepage : public QWidget
	{
		Q_OBJECT

	public:
		datasamplepage(QWidget *parent = nullptr);
		~datasamplepage();

	signals:
		void signalDataGather();
		void signalInspectData();
	protected:
		void contextMenuEvent(QContextMenuEvent *event);


	private slots:
		void slotLeadingInButton();
		void slotInspectAction();
	private:
		void init();
		void crerateAction();

	private:
		Base::RTableView * m_tableView;
		datasamplemodel *m_tableModel;

		Base::RIconButton * m_leadingInButton;
		Base::RIconButton * m_gatherButton;
		Base::RIconButton * m_identificationQueueButton;
		Base::RIconButton * m_retrievalButton;
		Base::RIconButton * m_congnizanceButton;
		Base::RIconButton * m_batchDeleButton;

		QMenu *m_pop_menu;
		QAction *m_hcAction;
		QAction *m_csbAction;
		QAction *m_swtzAction;
		QAction *m_mbxxAction;
		QAction *m_sbxxAction;
		QAction *m_scAction;
	};

}//namespace Related 