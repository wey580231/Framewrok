#include "usermanagemodel.h"

namespace Related {

	UserManageModel::UserManageModel(QObject *parent)
		: Base::RTableModel(parent)
	{
	}

	UserManageModel::~UserManageModel()
	{
	}

	int UserManageModel::datasSize() const
	{
		return m_dataList.size();
	}

	void UserManageModel::updateData(const QList<Datastruct::UserEntityData> & dlist)
	{
		m_dataList.clear();
		m_dataList.append(dlist);

		refresh();
	}

	QVariant UserManageModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		UserManageColumnIndex cindex = static_cast<UserManageColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Datastruct::UserEntityData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case U_Id:
				return data.id;
				break;
			case U_UserName:
				return data.name;
				break;
			case U_RegisterTime:
				return data.registTime;
				break; 
			case U_LastLoginTime:
				return data.registTime;
				break;
			case U_UserRights:
				return data.privilege;
				break;
			default:
				break;
			}
		}

		return QVariant();
	}
}//namespace Related 