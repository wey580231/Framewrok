#include "levelmodel1.h"

namespace Related {

	LevelModel1::LevelModel1(QObject *parent)
		: Base::RTableModel(parent)
	{

	}

	LevelModel1::~LevelModel1()
	{
	}

	QVariant LevelModel1::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		Level0ColumnIndex cindex = static_cast<Level0ColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Level1Data data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case L1_Id:
				return data.m_id;
			case L1_Num:
				return data.m_num;
			case L1_Name:
				return data.m_name;
			case L1_Timestamp:
				return data.m_timestamp;
			case L1_PlatformName:
				return data.m_platformName;
			case L1_TimeLength:
				return data.m_timeLength;
			case L1_DataType:
				return data.m_datatype;
			default:
				break;
			}
		}

		return QVariant();
	}

	QVariant LevelModel1::customProcessRole(int role, int rowIndex, int dataIndex, int columnId) const
	{
		if (role == Qt::SizeHintRole) {
			return QSize(220,40);
		}
		return QVariant();
	}

	void LevelModel1::prepareData()
	{
		for (int i = 0; i < 60; i++) {
			Level1Data ld;
			ld.m_id = i + 1;
			ld.m_num = QString("%1").arg(100000 + i);
			ld.m_name = QStringLiteral("Name_%1").arg(i + 1);
			ld.m_timestamp = QStringLiteral("2021-01-10 12:00:0%1").arg(i);
			ld.m_platformName = QStringLiteral("XXƽ̨%1").arg(i);
			ld.m_timeLength = QString("120");
			ld.m_datatype = QString("mp3");
			m_dataList.append(ld);
		}

		refresh();
	}

	int LevelModel1::datasSize() const
	{
		return m_dataList.size();
	}

} //namespace Related 
