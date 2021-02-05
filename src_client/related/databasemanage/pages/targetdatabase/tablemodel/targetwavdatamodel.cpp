#include "targetwavdatamodel.h"

#include <QDebug>

namespace Related {

	TargetWavDataModel::TargetWavDataModel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();
	}

	TargetWavDataModel::~TargetWavDataModel()
	{
	}

	void TargetWavDataModel::prepareData(QList<Datastruct::TargetWavEntityData> listData)
	{
		m_dataList.clear();
		m_dataList.append(listData);
		refresh();
	}

	int TargetWavDataModel::datasSize() const
	{
		return m_dataList.size();
	}

	QVariant TargetWavDataModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		TargetDataColumnIndex cindex = static_cast<TargetDataColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Datastruct::TargetWavEntityData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case TWAV_Index:
				return dataIndex + 1;
				break;
			case TWAV_TargetName:
				return data.targetName;
				break;
			case TWAV_Edttime:
				return data.edttime;
				break;
			case TWAV_Tonnage:
				return data.tonnage;
				break;
			case TWAV_Lon:
				return data.lon;
				break;
			case TWAV_Lat:
				return data.lat;
				break;
			case TWAV_Speed:
				return data.speed;
				break;
			case TWAV_AxlesNumber:
				return data.axlesNumber;
				break;
			case TWAV_Type:
				if (data.type == 0) {
					return QStringLiteral("Ãñ´¬");
				}
				else if (data.type == 1) {
					return QStringLiteral("¾ü´¬");
				}
				break;
			default:
				break;
			}
		}
		return QVariant();
	}

}//namespace Related 
