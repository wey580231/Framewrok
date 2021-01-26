#include "trialsheetmodel.h"

#include <QDebug>

namespace Related {

	TrialSheetModel::TrialSheetModel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();
	}

	TrialSheetModel::~TrialSheetModel()
	{
	}

	void TrialSheetModel::prepareData(QList<Datastruct::ExperimentRecordEntityData> listInfos)
	{
		m_dataList.clear();
		m_dataList.append(listInfos);

		refresh();
	}

	int TrialSheetModel::datasSize() const
	{
		return m_dataList.size();
	}

	QVariant TrialSheetModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		TrialSheetDataColumnIndex cindex = static_cast<TrialSheetDataColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Datastruct::ExperimentRecordEntityData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case TS_Index:
				return dataIndex;
				break;
			case TS_PlatformId:
				return data.platformId;
				break;
			case TS_Lon:
				return data.lon;;
				break;
			case TS_Lat:
				return data.lat;
				break;
			case TS_SeaCondition:
				return data.seaCondition;
				break;
			case TS_FloatingTime:
				return data.floatingTime;
				break;
			default:
				break;
			}
		}

		return QVariant();
	}

}//namespace Related 
