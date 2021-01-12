/*!
 * @brief     通用表格模型
 * @details	  1.支持动态列的调整，包括列顺序调整
			  2.提供快速建立表格数据显示的能力
 * @author    wey
 * @version   1.0
 * @date      2021.01.08 16:36:43
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QObject>
#include <QAbstractTableModel>
#include <QList>

#include "../../base_global.h"
#include "tablestruct.h"

namespace Base {

	class BASESHARED_EXPORT RTableModel : public QAbstractTableModel
	{
		Q_OBJECT

	public:
		RTableModel(QObject *parent = nullptr);
		~RTableModel();

		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const;

		/*!
		 * @brief 批量更新当前表格列配置
		 * @param items 最新列配置
		 */
		void updateColumnItems(ColumnItems items);
		QList<ColumnItem> getColumns() { return m_columns; }

		void setColumVisible(int id,bool visible);
		int visibleColumnCount();

		void refresh();

		/*!
		 * @brief 设置页面数据显示条数
		 * @param nums >0 表示每页至多显示条数，<0表示不限制
		 */
		void setPageItemCount(int nums);

	protected:
		/*!
		 * @brief 获取指定行、列的显示数据
		 * @param rowIndex 当前行号
		 * @param dataIndex 数据在整个数据集合中的索引
		 * @param columnId 列标识，可通过此标识区分列
		 * @return 单元格显示内容
		 */
		virtual QVariant displayData(int rowIndex,int dataIndex,int columnId) const;

		/*!
		 * @brief 自定义处理单元格属性
		 * @param rowIndex 当前行号
		 * @param dataIndex 数据在整个数据集合中的索引
		 * @param columnId 列标识，可通过此标识区分列
		 * @return 单元格显示内容
		 */
		virtual QVariant customProcessRole(int role, int rowIndex, int dataIndex, int columnId) const;

		ColumnItem getVisibleItem(int index) const;

	protected:
		ColumnItems m_columns;

		int m_itemsPerPage;		/*!< 当前页面显示条数 >0 表示每页至多显示条数，<0表示不限制 */
		int m_pageCount;		/*!< 当前页数，从0开始 */
	};


} //namespace Base 