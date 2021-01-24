#include "dataoverviewmolel.h"

namespace Related {

	DataOverviewMolel::DataOverviewMolel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();
	}

	DataOverviewMolel::~DataOverviewMolel()
	{
	}

	void DataOverviewMolel::prepareData()
	{
		for (int i = 0; i < 10; i++) {
			DataOverviewData ld;
			ld.index = i + 1;
			ld.fileName = QString("Name_%1").arg(i + 1);
			ld.platformName = QString("2021-01-10 12:00:0%1").arg(i);
			ld.dataSize = i + 1;;
			ld.endIndex = i;
			m_dataList.append(ld);
		}

		refresh();
	}

	int DataOverviewMolel::datasSize() const
	{
		return m_dataList.size();
	}

	QVariant DataOverviewMolel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		DataOverviewColumnIndex cindex = static_cast<DataOverviewColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			DataOverviewData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case DO_Index:
				return data.index;
				break;
			case DO_FileName:
				return data.fileName;
				break;
			case DO_PlatformName:
				return data.platformName;
				break;
			case DO_PlatformType:
				return data.platformType;
				break;
			case Do_DataSize:
				return data.dataSize;
				break;
			case DO_StartIndex:
				return data.startIndex;
				break;
			case DO_EndIndex:
				return data.endIndex;
				break;
			default:
				break;
			}
		}

		return QVariant();
	}

}//namespace Related 
