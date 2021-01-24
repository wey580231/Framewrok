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

	void LogbookModel::prepareData()
	{
		for (int i = 0; i < 10; i++) {
			LogbookData ld;
			ld.index = i + 1;
			ld.tsetName = QString("Name_%1").arg(i + 1);
			ld.edttime = QString("2021-01-10 12:00:0%1").arg(i);
			ld.number = i + 1;;
			ld.type = i;
			m_dataList.append(ld);
		}

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
			LogbookData data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case L_Index:
				return data.index;
				break;
			case L_Number:
				return data.number;
				break;
			case L_CreateTime:
				return data.edttime;
				break;
			case L_TestName:
				return data.tsetName;
				break;
			case L_PlatformName:
				return data.platformName;
				break;
			case L_TimeLength:
				return data.timeLength;
				break;
			case L_Type:
				return data.type;
				break;
			default:
				break;
			}
		}

		return QVariant();
	}

}// namespace Related 