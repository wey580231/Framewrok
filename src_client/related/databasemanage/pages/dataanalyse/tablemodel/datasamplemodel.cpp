#include "datasamplemodel.h"

namespace Related {

	datasamplemodel::datasamplemodel(QObject *parent)
		: Base::RTableModel(parent)
	{

	}

	datasamplemodel::~datasamplemodel()
	{

	}

	int datasamplemodel::rowCount(const QModelIndex & parent) const
	{
		return m_dataList.size();
	}

	void datasamplemodel::prepareData()
	{
		for (int i = 0; i < 30; i++) {
			Level0Data ld;
			ld.m_id = i + 1;
			ld.m_name = QString("Name_%1").arg(i + 1);
			ld.m_timestamp = QString("2021-01-10 12:00:0%1").arg(i);
			m_dataList.append(ld);
		}

		refresh();
	}

	QVariant datasamplemodel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		Level0ColumnIndex cindex = static_cast<Level0ColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Level0Data data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case D_Id:
				return data.m_id;
				break;
			case D_BatchNumber:
				return data.m_name;
				break;
			case D_Time:
				return data.m_timestamp;
				break;
			default:
				break;
			}
		}

		return QVariant();
	}

	QVariant datasamplemodel::customProcessRole(int role, int rowIndex, int dataIndex, int columnId) const
	{
		if (role == Qt::SizeHintRole) {
			return QSize(220, 40);
		}
		return QVariant();
	}

}//namespace Related 