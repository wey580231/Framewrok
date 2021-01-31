#include "targetdatabasemanagemodel.h"

namespace Related {

	TargetDatabaseManageModel::TargetDatabaseManageModel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();

	}

	TargetDatabaseManageModel::~TargetDatabaseManageModel()
	{
	}

	void TargetDatabaseManageModel::prepareData()
	{
		for (int i = 0; i < 10; i++) {
			TargetDatabaseData ld;
			ld.index = i + 1;
			ld.targetName = QString("%1_%2").arg(QStringLiteral("²âÊÔ´¬")).arg(i+1);
			ld.edttime = QString("2021-01-10 12:00:0%1").arg(i);
			ld.tonnage = 11;
			ld.lon = 16;
			ld.lat = 18 +i;
			ld.speed = 16;
			ld.axlesNumber = 5;
			ld.type = 0;
			m_dataList.append(ld);
		}

		refresh();
	}

	int TargetDatabaseManageModel::datasSize() const
	{
		return m_dataList.size();
	}

	QVariant TargetDatabaseManageModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		TargetDatabaseDataColumnIndex cindex = static_cast<TargetDatabaseDataColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			TargetDatabaseData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case T_Index:
				return data.index;
				break;
			case T_TargetName:
				return data.targetName;
				break;
			case T_Edttime:
				return data.edttime;
				break;
			case T_Tonnage:
				return data.tonnage;
				break;
			case T_Lon:
				return data.lon;
				break;
			case T_Lat:
				return data.lat;
				break;
			case T_Speed:
				return data.speed;
				break;
			case T_AxlesNumber:
				return data.axlesNumber;
				break;
			case T_Type:
				if (data.type == 0) {
					return QStringLiteral("Ãñ´¬");
				}
				else if(data.type == 1)
				{
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