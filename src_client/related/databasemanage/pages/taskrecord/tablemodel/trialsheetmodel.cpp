#include "trialsheetmodel.h"

namespace Related {

	TrialSheetModel::TrialSheetModel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();
	}

	TrialSheetModel::~TrialSheetModel()
	{
	}

	void TrialSheetModel::prepareData()
	{
		for (int i = 0; i < 10; i++) {
			TrialSheetData ld;
			ld.index = i + 1;
			ld.platformName = QString("Name_%1").arg(i + 1);
			ld.edttime = QString("2021-01-10 12:00:0%1").arg(i);
			ld.index = i + 1;;
			ld.type = i;
			m_dataList.append(ld);
		}

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
			TrialSheetData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case TS_Index:
				return data.index;
				break;
			case TS_PlatformName:
				return data.platformName;
				break;
			case TS_Type:
				return data.type;
				break;
			case TS_Edttime:
				return data.edttime;
				break;
			default:
				break;
			}
		}

		return QVariant();
	}

}//namespace Related 
