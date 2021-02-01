#include "aisdatamodel.h"

namespace Related {

	AisDataModel::AisDataModel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();
	}

	AisDataModel::~AisDataModel()
	{
	}

	void AisDataModel::prepareData(QList<Datastruct::AisEntityData> listData)
	{
		m_dataList.clear();
		m_dataList.append(listData);
		refresh();
	}

	int AisDataModel::datasSize() const
	{
		return m_dataList.size();
	}

	QVariant AisDataModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		AISDATAColumnIndex cindex = static_cast<AISDATAColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Datastruct::AisEntityData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case AIS_Index:
				return dataIndex + 1;
				break;
			case AIS_Mmsi:
				return data.mmsi;
				break;
			case AIS_Time:
				return data.time;
				break;
			case AIS_Lon:
				return data.lon;
				break;
			case AIS_Lat:
				return data.lat;
				break;
			case AIS_Course:
				return data.course;
				break;
			case AIS_Truehead:
				return data.truehead;
				break;
			case AIS_Name:
				return data.name;
				break;
			case AIS_ShipType:
				return data.shipType;
				break;
			case AIS_NavStatus: {
				if(data.navStatus == 1){
					return QStringLiteral("Õý³£");
				}
				else
				{
					return  QStringLiteral("Òì³£");
				}
			}
				break;
			case AIS_SPEED:
				return data.speed;
				break;
			case AIS_LENGTH:
				return data.length;
				break;
			case AIS_WIDTH:
				return data.width;
				break;
			case AIS_BUILDDATE:
				return data.buildDate;
				break;
			case AIS_PORT:
				return data.port;
				break;
			default:
				break;
			}
		}
		return QVariant();
	}

}//namespace Related 