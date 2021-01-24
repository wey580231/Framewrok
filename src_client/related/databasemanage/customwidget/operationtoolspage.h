/*!
 *  @brief    操作工具页面
 *  @details  该页面提供 增加、删除、修改、刷新
 *  @author    yzg
 *  @version   1.0
 *  @date      2021.1.14
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QObject>
#include <QWidget>
#include <qlabel.h>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <base\selfwidget\iconbutton.h>

namespace Related {

	class OperationToolsPage : public QWidget
	{
		Q_OBJECT

	public:
		OperationToolsPage(QWidget *parent = nullptr);
		~OperationToolsPage();

		enum ButtType {
			Butt_Add,		/*!< 添加 */
			Butt_Delete,	/*!< 删除 */
			Butt_Edit,		/*!< 修改 */
			Butt_Refresh	/*!< 刷新 */
		};

	signals:
		void buttPressed(ButtType type);

	private slots:
		void respButtCliecked();

	private:
		void  init();

	private:
		Base::RIconButton * m_addButton;
		Base::RIconButton * m_delButton;
		Base::RIconButton * m_editButton;
		Base::RIconButton * m_refreshButton;

		Base::RIconButton * m_searchButton;
		Base::RIconButton * m_advanceSearchButton;
		Base::RIconButton * m_moreButton;

	};

}//namespace Related 