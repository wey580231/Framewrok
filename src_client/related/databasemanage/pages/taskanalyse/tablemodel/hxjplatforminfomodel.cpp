#include "hxjplatforminfomodel.h"

#include <QDebug>

namespace Related {

	HXJPlatformInfoModel::HXJPlatformInfoModel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();
	}

	HXJPlatformInfoModel::~HXJPlatformInfoModel()
	{
	}

	void HXJPlatformInfoModel::prepareData()
	{
		for (int i = 0; i < 10; i++) {
			HXJPlatformInfoData ld;
			ld.index = i + 1;
			ld.platformName = QString("Name_%1").arg(i + 1);
			ld.inputTime = QString("2021-01-10 12:00:0%1").arg(i);
			ld.dataIndex = i + 1;;
			ld.courseAngle = i;
			m_dataList.append(ld);
		}

		refresh();
	}

	int HXJPlatformInfoModel::datasSize() const
	{
		return m_dataList.size();
	}

	QVariant HXJPlatformInfoModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		HXJPlatformInfoColumnIndex cindex = static_cast<HXJPlatformInfoColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			HXJPlatformInfoData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case HXJPI_Index:
				return data.index;
				break;
			case HXJPI_PlatformName:
				return data.platformName;
				break;
			case HXJPI_DataIndex:
				return data.dataIndex;
				break;
			case HXJPI_InputTime:
				return data.inputTime;
				break;
			case HXJPI_CourseAngle:
				return data.courseAngle;
				break;
			case HXJPI_PitchAngle:
				return data.pitchAngle;
				break;
			case HXJPI_RollAngle:
				return data.rollAngle;
				break;
			default:
				break;
			}
		}

		return QVariant();
	}

}//namespace Related