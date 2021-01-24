#include "ambientnoisemodel.h"

#include <QDebug>

namespace Related {

	AmbientNoiseModel::AmbientNoiseModel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();
	}

	AmbientNoiseModel::~AmbientNoiseModel()
	{
	}

	void AmbientNoiseModel::prepareData()
	{
		for (int i = 0; i < 10; i++) {
			AmbientNoiseData ld;
			ld.index = i + 1;
			ld.fileName = QString("Name_%1").arg(i + 1);
			ld.platformName = QString("2021-01-10 12:00:0%1").arg(i);
			ld.timeLength = i + 1;;
			ld.dataSize = i;
			m_dataList.append(ld);
		}

		refresh();
	}

	int AmbientNoiseModel::datasSize() const
	{
		return m_dataList.size();
	}

	QVariant AmbientNoiseModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		AmbientNoiseColumnIndex cindex = static_cast<AmbientNoiseColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			AmbientNoiseData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case AN_Index:
				return data.index;
				break;
			case AN_FileName:
				return data.fileName;
				break;
			case AN_PlatformName:
				return data.platformName;
				break;
			case AN_TimeLength:
				return data.timeLength;
				break;
			case AN_DataSize:
				return data.dataSize;
				break;
			case AN_StartIndex:
				return data.startIndex;
				break;
			case AN_EndIndex:
				return data.endIndex;
				break;
			default:
				break;
			}
		}

		return QVariant();
	}

}//namespace Related 
