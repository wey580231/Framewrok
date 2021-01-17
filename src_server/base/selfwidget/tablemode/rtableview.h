/*!
 * @brief     自定义表格视图
 * @details   1.支持表格列手动调整
			  2.支持列排序
			  3.支持记录当前列宽度，当列隐藏显示后，可确保列的宽度信息不变
 * @author    wey
 * @version   1.0
 * @date      2021.01.08 16:44:51
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QTableView>
#include <QMenu>

#include "../../base_global.h"
#include "tablestruct.h"

namespace Base {

	class RTableModel;

	class BASESHARED_EXPORT RTableView : public QTableView
	{
		Q_OBJECT

	public:
		RTableView(QWidget *parent = nullptr);
		~RTableView();

		/*!
		 * @brief 设置表格数据模型
		 * @attention 1.此方法会覆盖QTableView的同名方法，只接受自定义的模型
		 *			  2.只有先调用setModel()，设置表格的列宽才会生效
		 * @param model 自定义表格模型
		 */
		void setModel(RTableModel *model);

		/*!
		 * @brief 表格功能项
		 */
		enum TableFeature {
			T_ColumnEdit = 0x0001,			/*!< 表格编辑(列排序、列显隐) */
			T_HeadSorting = 0x0002, 		/*!< 表头排序 */
			T_AlternatingRowColors = 0x0004	/*!< 隔行变色 */
		};
		Q_DECLARE_FLAGS(TableFeatures, TableFeature);
		
		/*!
		 * @brief 控制表格特性开启或关闭
		 * @param feature 待控制的表格特性
		 * @param enabled true:启用；false:关闭
		 */
		void setTableFeature(TableFeature feature,bool enabled);

		/*!
		 * @brief 添加表格列
		 * @param item 表格列信息描述
		 * @return true:添加成功，false:表示已经存在相同id或name的列
		 */
		bool addColumnItem(ColumnItem item);

	private slots:
		void respCustomContextMenu(const QPoint & point);

		void showColumnVisibleEditWindow(bool flag);
		void updateModelColumnVisible(bool flag);

		/*!
		 * @brief 表头点击排序
		 * @param column 点击的表头列编号
		 */
		void respSectionClicked(int column);

		/*!
		 * @brief 处理表格列改变
		 * @param logicalIndex 改变的索引编号
		 * @param oldSize 改变前列宽度/高度
		 * @param newSiz 改变后列宽度
		 */
		void respColumnResize(int logicalIndex, int oldSize, int newSiz);

	private:
		void udateFeature();
		void resizeColumnWidth();

	private:
		QMenu * m_headMenu;
		TableFeatures m_tableFeatures;
		int m_lastClickedColumn;
		bool m_ascending;		/*!< true：升序；false：降序 */

		ColumnItems m_columns;	/*!< 所有表格列 */
		bool m_columResizeByUser;		/*!< 用户手工调整列宽 */
	};

} //namespace Base 
