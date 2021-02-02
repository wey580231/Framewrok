#include "dutyrecordmodel.h"

#include <QDebug>

namespace Related {

	DutyRecordModel::DutyRecordModel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();
	}

	DutyRecordModel::~DutyRecordModel()
	{
	}

	void DutyRecordModel::prepareData(QList<Datastruct::DutyRecordEntityData> dutyRecordInfos)
	{
		m_dataList.clear();
		m_dataList.append(dutyRecordInfos);
		refresh();
	}

	int DutyRecordModel::datasSize() const
	{
		return m_dataList.size();
	}

	QVariant DutyRecordModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		DutyRecordDataColumnIndex cindex = static_cast<DutyRecordDataColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Datastruct::DutyRecordEntityData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case DR_Index:
				return dataIndex+1;
				break;
			case DR_CreateTime:
				return data.createTime;
				break;
			case DR_Description:
				return data.description;
				break;
			case DR_SeaCondition:
				return data.seaCondition;
				break;
			case DR_Wind:
				return data.wind;
				break;
			case DR_WindSpeed:
				return data.windSpeed;
				break;
			case DR_WaveHigh:
				return data.waveHigh;
				break;
			case DR_OceanCurrents:
				return data.oceanCurrents;
				break;
			default:
				break;
			}
		}
		return QVariant();
	}

}// namespace Related 