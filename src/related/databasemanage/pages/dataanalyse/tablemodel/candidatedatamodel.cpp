#include "candidatedatamodel.h"

namespace Related {

	CandidateDataModel::CandidateDataModel(QObject *parent)
		: Base::RTableModel(parent)
	{
	}

	CandidateDataModel::~CandidateDataModel()
	{
	}

	int CandidateDataModel::rowCount(const QModelIndex & parent) const
	{
		return m_dataList.size();
	}

	void CandidateDataModel::prepareData()
	{
		for (int i = 0; i < 10; i++) {
			Level0Data ld;
			ld.m_id = i + 1;
			ld.m_name = QString("Name_%1").arg(i + 1);
			ld.m_timestamp = QString("2021-01-10 12:00:0%1").arg(i);
			m_dataList.append(ld);
		}

		refresh();
	}

	QVariant CandidateDataModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		Level0ColumnIndex cindex = static_cast<Level0ColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Level0Data data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case L0_Id:
				return data.m_id;
				break;
			case L0_Num:
				return data.m_name;
				break;
			case L0_Timestamp:
				return data.m_timestamp;
				break;
			default:
				break;
			}
		}

		return QVariant();
	}

	QVariant CandidateDataModel::customProcessRole(int role, int rowIndex, int dataIndex, int columnId) const
	{
		if (role == Qt::SizeHintRole) {
			return QSize(220, 40);
		}
		return QVariant();
	}

}//namespace Related 