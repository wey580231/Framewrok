/*!
 * @brief     表格列编辑器
 * @details	  此类作为RTableView的配套功能类，提供对表格列的显示隐藏编辑，表格列顺序的调整
 * @attention 此类不对外提供接口
 * @author    wey
 * @version   1.0
 * @date      2021.01.09 16:27:39
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 
#pragma once

#include <QWidget>
#include <QListWidget>

#include "../dialogproxy.h"
#include "../iconbutton.h"
#include "tablestruct.h"

namespace Base {

	class TableColumnEditWindow : public DialogProxy
	{
		Q_OBJECT

	public:
		TableColumnEditWindow(QWidget *parent = nullptr);
		~TableColumnEditWindow();

	private slots:
		void respOk();
		void respCancel();

		void changeItemIndex();

	private:
		void setItems(ColumnItems & items);
		ColumnItems getItems() { return m_newItems; }

		void init();

	private:
		QListWidget * m_itemListWidget;
		RIconButton * m_moveUp;
		RIconButton * m_moveDown;

	private:
		ColumnItems m_newItems;

	private:
		friend class RTableView;
	};

} //namespace Base 