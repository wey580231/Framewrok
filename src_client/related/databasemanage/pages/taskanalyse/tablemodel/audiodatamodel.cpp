#include "audiodatamodel.h"

#include <QDebug>

namespace Related {

	AudioDataModel::AudioDataModel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();
	}

	AudioDataModel::~AudioDataModel()
	{
	}

	void AudioDataModel::prepareData()
	{
		for (int i = 0; i < 10; i++) {
			AudioData ld;
			ld.index = i + 1;
			ld.name = QString("Name_%1").arg(i + 1);
			ld.platformName = QString("2021-01-10 12:00:0%1").arg(i);
			ld.dataSize = i + 1;;
			ld.timeLength = i;
			m_dataList.append(ld);
		}

		refresh();
	}

	int AudioDataModel::datasSize() const
	{
		return m_dataList.size();
	}

	QVariant AudioDataModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		AudioDataColumnIndex cindex = static_cast<AudioDataColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			AudioData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case AD_Index:
				return data.index;
				break;
			case AD_Name:
				return data.name;
				break;
			case AD_TimeLength:
				return data.timeLength;
				break;
			case AD_PlatformName:
				return data.platformName;
				break;
			case AD_DataSize:
				return data.dataSize;
				break;
			default:
				break;
			}
		}

		return QVariant();
	}

}//namespace Related 