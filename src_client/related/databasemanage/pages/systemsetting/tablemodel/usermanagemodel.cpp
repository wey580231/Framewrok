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

	int UserManageModel::getRecordDatabaseIndex(int row)
	{
		if (row < 0 || row > m_dataList.size())
			return -1;

		return m_dataList.at(row).id;
	}

	int UserManageModel::getUserPrivilege(int row)
	{
		if (row < 0 || row > m_dataList.size())
			return -1;

		return m_dataList.at(row).privilege;
	}

	bool UserManageModel::isUserManage(int row)
	{
		if (row < 0 || row > m_dataList.size())
			return false;

		return m_dataList.at(row).isManager;
	}

	QVariant UserManageModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		UserManageColumnIndex cindex = static_cast<UserManageColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Datastruct::UserEntityData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
				case U_Id:
					return dataIndex + 1;
					break;
				case U_UserName:
					return data.name;
					break;
				case U_RegisterTime:
					return data.registTime;
					break;
				case U_LastLoginTime:
					return data.lastLoadTime;
					break;
				case U_UserRights:
					if (data.privilege == Datastruct::NonePrivilege)
						return QStringLiteral("无权限");
					else {
						QStringList pvlist;
						uint mask = Datastruct::ReadOnly;
						while (mask <= Datastruct::AllPrivilege)
						{
							uint sb = data.privilege & mask;
							mask <<= 1;
							if (!sb)
								continue;

							Datastruct::UserPrivilege up = static_cast<Datastruct::UserPrivilege>(sb);

							switch (up)
							{
								case Datastruct::ReadOnly:pvlist.append(QStringLiteral("可查看")); break;
								case Datastruct::ReadWrite:pvlist.append(QStringLiteral("可编辑")); break;
								case Datastruct::DeleteAble:pvlist.append(QStringLiteral("可删除")); break;
								default:
									break;
							}
						}

						if (pvlist.size() > 0) {
							return pvlist.join("/ ");
						}
						else {
							return QStringLiteral("无权限");
						}
					}

					return data.privilege;
					break;
				case U_IsManage:
					return data.isManager == 1 ? QStringLiteral("是") : QStringLiteral("--");
					break;
				default:
					break;
			}
		}

		return QVariant();
	}
}//namespace Related 