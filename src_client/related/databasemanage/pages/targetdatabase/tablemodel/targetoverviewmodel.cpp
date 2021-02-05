#include "targetoverviewmodel.h"

namespace Related {

	TargetOverViewModel::TargetOverViewModel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();

	}

	TargetOverViewModel::~TargetOverViewModel()
	{
	}

	void TargetOverViewModel::prepareData(QList<Datastruct::TargetEntityData> listData)
	{
		m_dataList.clear();
		m_dataList.append(listData);
		refresh();
	}

	int TargetOverViewModel::datasSize() const
	{
		return m_dataList.size();
	}

	QVariant TargetOverViewModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		TargetDataColumnIndex cindex = static_cast<TargetDataColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Datastruct::TargetEntityData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case T_Index:
				return dataIndex + 1;
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
				} else if(data.type == 1) {
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