#include "logbookmodel.h"

#include <QDebug>

namespace Related {

	LogbookModel::LogbookModel(QObject *parent)
		: Base::RTableModel(parent)
	{
		m_dataList.clear();
	}

	LogbookModel::~LogbookModel()
	{
	}

	void LogbookModel::prepareData(QList<Datastruct::DutyRecordEntityData> dutyRecordInfos)
	{
		m_dataList.clear();
		m_dataList.append(dutyRecordInfos);

		refresh();
	}

	int LogbookModel::datasSize() const
	{
		return m_dataList.size();
	}

	QVariant LogbookModel::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		LogbookDataColumnIndex cindex = static_cast<LogbookDataColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Datastruct::DutyRecordEntityData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case L_Index:
				return dataIndex;
				break;
			case L_CreateTime:
				return data.createTime;
				break;
			case L_Description:
				return data.description;
				break;
			case L_SeaCondition:
				return data.seaCondition;
				break;
			default:
				break;
			}
		}

		return QVariant();
	}

}// namespace Related 