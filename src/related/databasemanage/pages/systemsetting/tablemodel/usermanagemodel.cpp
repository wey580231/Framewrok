#include "usermanagemodel.h"

namespace Related {

	UserManageModel::UserManageModel(QObject *parent)
		: Base::RTableModel(parent)
	{
	}

	UserManageModel::~UserManageModel()
	{
	}

	void UserManageModel::prepareData()
	{
		for (int i = 0; i < 10; i++) {
			UserManageData ld;
			ld.m_id = i + 1;
			ld.m_name = QString("Name_%1").arg(i + 1);
			ld.m_timestamp = QString("2021-01-10 12:00:0%1").arg(i);
			m_dataList.append(ld);
		}

		refresh();
	}

	int UserManageModel::datasSize() const
	{
		return m_dataList.size();
	}

	QVariant UserManageModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		UserManageColumnIndex cindex = static_cast<UserManageColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			UserManageData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case U_Id:
				return data.m_id;
				break;
			case U_UserName:
				return data.m_name;
				break;
			case U_RegisterTime:
				return data.m_timestamp;
				break;
			default:
				break;
			}
		}

		return QVariant();
		return QVariant();
	}

	QVariant UserManageModel::customProcessRole(int role, int rowIndex, int dataIndex, int columnId) const
	{
		return QVariant();
	}

}//namespace Related 