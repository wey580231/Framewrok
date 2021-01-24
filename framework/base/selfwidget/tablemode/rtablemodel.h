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

	typedef std::function<bool(const QVariant & va, const QVariant & vb)> CustomSortCallback;

	class BASESHARED_EXPORT RTableModel : public QAbstractTableModel
	{
		Q_OBJECT

	public:
		RTableModel(QObject *parent = nullptr);
		~RTableModel();

		virtual int rowCount(const QModelIndex &parent = QModelIndex() ) const;
		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const;

		int getPageNum() const { return m_pageNum; }

		/*!
		 * @brief 批量更新当前表格列配置
		 * @param items 最新列配置
		 */
		void updateColumnItems(ColumnItems items);
		QList<ColumnItem> getColumns() { return m_columns; }

		void setColumVisible(int id,bool visible);
		int visibleColumnCount();
		
		/*!
		 * @brief 对当前显示数据，指定列排序
		 * @param column 待排序的列编号
		 * @param ord 排列顺序
		 */
		void sortByColumn(int column, Qt::SortOrder ord);

		/*!
		 * @brief 为指定列设置排序策略
		 * @param columnId 待设置回调函数的列
		 * @param callback 自定义排序策略
		 */
		void addCustomColumSortStragety(int columnId, CustomSortCallback callback);

		/*!
		 * @brief 刷新表格数据
		 * @details 1.若表格处于排序状态，则会进行排序
					2.刷新表格显示
		 */
		void refresh();

	public slots:
		/*!
		 * @brief 设置表格固定显示行数
		 * @note 若设置固定显示行，则子类无需覆写rowCount()方法。
		 * @param rowNums 新设置的固定行，必须大于0
		 */
		void setFixedPageRowCount(int rowNums);

		/*!
		 * @brief 设置当前页面索引
		 * @attention 只有将所有需要显示的数据一次性设置入model中，才可以启用此功能
		 * @param page 页面索引编号，从0开始。
		 */
		void setPageNum(int page);

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

		/*!
		 * @brief 真正执行排序的代码
		 */
		void executeSort();

		ColumnItem getVisibleItem(int index) const;

	protected:
		ColumnItems m_columns;

		int m_itemsPerPage;		/*!< 当前页面显示条数 >0 表示每页至多显示条数，<0表示不限制 */
		int m_pageNum;		    /*!< 当前页数，从0开始 */
		int m_perPageCount;		/*!< 每页固定行数 */

		QList<int> m_sortedRows;	/*!< 参与排序数据排序后，每行数据对应的序号 */
		int m_sortedColumn;			/*!< 当前排序列索引，>= 0表示排序有效 */
		Qt::SortOrder m_sortOrder;	/*!< 当前表格排序顺序 */
		QMap<int, CustomSortCallback> m_sortCallback;		/*!< 指定列排序策略，若当前列没有设置，则使用默认排序策略 */
	};

} //namespace Base 