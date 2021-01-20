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
			ld.targetName = QString("Name_%1").arg(i + 1);
			ld.edttime = QString("2021-01-10 12:00:0%1").arg(i);
			ld.tonnage = i + 1;;
			ld.axlesNumber = i;
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
			case T_AxlesNumber:
				return data.axlesNumber;
				break;
			case T_Datalength:
				return data.datalength;
				break;
			case T_Type:
				return data.type;
				break;
			default:
				break;
			}
		}

		return QVariant();
	}

}//namespace Related 