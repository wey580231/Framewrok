#include "experimentrecordmodel.h"

#include <QDebug>

namespace Related {

	ExperimentRecordModel::ExperimentRecordModel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();
	}

	ExperimentRecordModel::~ExperimentRecordModel()
	{
	}

	void ExperimentRecordModel::prepareData(QList<Datastruct::ExperimentRecordEntityData> listInfos)
	{
		m_dataList.clear();
		m_dataList.append(listInfos);

		refresh();
	}

	int ExperimentRecordModel::datasSize() const
	{
		return m_dataList.size();
	}

	QVariant ExperimentRecordModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		ExperimentRecordDataColumnIndex cindex = static_cast<ExperimentRecordDataColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Datastruct::ExperimentRecordEntityData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case ER_Index:
				return dataIndex + 1;
				break;
			case ER_PlatformId:
				return data.platformId;
				break;
			case ER_FloatingTime:
				return data.floatingTime;
				break;
			case ER_Lon:
				return data.lon;
				break;
			case ER_Lat:
				return data.lat;
				break;
			case ER_SetHeadingDegree:
				return data.setHeadingDegree;
				break;
			case ER_ActualHeadingDegree:
				return data.actualHeadingDegree;
				break;
			case ER_AcousticState:
				return data.acousticState;
				break;
			case ER_TargetNum:
				return data.targetNum;
				break;
			case ER_UnderwaterTargetNum:
				return data.underwaterTargetNum;
				break;
			case ER_UnderwaterTargetInfo:
				return data.underwaterTargetInfo;
				break;
			case ER_MaxDepth:
				return data.maxDepth;
				break;
			case ER_ProfileIndex:
				return data.profileIndex;
				break;
			case ER_ProfileLength:
				return data.profileLength;
				break;
			case ER_ProfileDistance:
				return data.profileDistance;
				break;
			default:
				break;
			}
		}

		return QVariant();
	}

}//namespace Related 
