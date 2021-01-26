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
#include <QMap>

#include <base\selfwidget\iconbutton.h>
#include <base\macros.h>

namespace Related {

	class OperationToolsPage : public QWidget
	{
		Q_OBJECT

	public:
		OperationToolsPage(QWidget *parent = nullptr);
		~OperationToolsPage();

		enum ButtType {
			Butt_Add = 0x0001,		/*!< 添加 */
			Butt_Delete = 0x0002,	/*!< 删除 */
			Butt_Edit = 0x0004,		/*!< 修改 */
			Butt_Refresh = 0x0008	/*!< 刷新 */
		};
		Q_DECLARE_FLAGS(ButtTypes, ButtType)
		Q_DECLARE_FRIEND_FLAGS(ButtTypes)

		/*! 
		 * @brief 设置多个按钮显隐
		 * @param butts 多个按钮对应的枚举值集合
		 * @param visible true:按钮显示；false:按钮隐藏
		 */
		void setButtVisible(ButtTypes butts,bool visible = false);

	signals:
		void buttPressed(OperationToolsPage::ButtType type);

	private slots:
		void respButtCliecked();

	private:
		void  init();

	private:
		Base::RIconButton * m_addButton;
		Base::RIconButton * m_delButton;
		Base::RIconButton * m_editButton;
		Base::RIconButton * m_refreshButton;

		QMap<ButtType, Base::RIconButton *> m_buttMap;
	};

}//namespace Related 