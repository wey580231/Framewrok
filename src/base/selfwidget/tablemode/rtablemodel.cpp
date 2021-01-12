#include "rtablemodel.h"

namespace Base {

	RTableModel::RTableModel(QObject *parent)
		: QAbstractTableModel(parent)
	{
		
	}

	RTableModel::~RTableModel()
	{
	}
 
	int RTableModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
	{
		int visibleCount = std::count_if(m_columns.begin(), m_columns.end(), [](const ColumnItem & item) {
			return item.m_visible;
		});

		return visibleCount;
	}

	QVariant RTableModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
	{
		if (!index.isValid() || index.column() > m_columns.size())
			return QVariant();

		ColumnItem && item = getVisibleItem(index.column());

		if (role == Qt::DisplayRole) {
			return displayData(index.row(), index.row(), item.m_id);
		}
		else if (role == Qt::TextAlignmentRole) {
			return item.m_align;
		}

		return customProcessRole(role,index.row(),0,item.m_id);
	}

	QVariant RTableModel::headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const
	{
		if (section > m_columns.size())
			return QVariant();

		ColumnItem && item = getVisibleItem(section);

		if (orientation == Qt::Horizontal) {
			if (role == Qt::DisplayRole) {
				return item.m_name;
			}
			else if (role == Qt::TextAlignmentRole) {
				return item.m_align;
			}
		}

		return QVariant();
	}

	void RTableModel::updateColumnItems(ColumnItems items)
	{
		beginResetModel();
		m_columns.swap(items);
		endResetModel();
	}

	void RTableModel::setColumVisible(int id, bool visible)
	{
		for (int i = 0; i < m_columns.size(); i++) {
			if (m_columns[i].m_id == id) {
				m_columns[i].m_visible = visible;
				break;
			}
		}
	}

	int RTableModel::visibleColumnCount()
	{
		return std::count_if(m_columns.begin(), m_columns.end(), [](const ColumnItem & item) {
			return item.m_visible;
		});
	}

	void RTableModel::refresh()
	{
		beginResetModel();
		endResetModel();
	}

	void RTableModel::setPageItemCount(int nums)
	{
		m_itemsPerPage = nums;
	}

	QVariant RTableModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		return QVariant();
	}

	QVariant RTableModel::customProcessRole(int role,int rowIndex, int dataIndex, int columnId) const
	{
		return QVariant();
	}

	Base::ColumnItem RTableModel::getVisibleItem(int index) const
	{
		int loop = 0;
		for (int i = 0; i < m_columns.size(); i++) {
			if (m_columns[i].m_visible) {
				if (loop++ == index)
					return m_columns[i];
			}
		}
	}

} //namespace Base 