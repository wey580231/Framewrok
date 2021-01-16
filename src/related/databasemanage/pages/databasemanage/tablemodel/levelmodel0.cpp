#include "levelmodel0.h"

namespace Related {

	LevelModel0::LevelModel0(QObject *parent)
		: Base::RTableModel(parent)
	{

	}

	LevelModel0::~LevelModel0()
	{
	}

	QVariant LevelModel0::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		Level0ColumnIndex cindex = static_cast<Level0ColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Level0Data data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case L0_Id:
				return data.m_id;
			case L0_Num:
				return data.m_num;
			case L0_Name:
				return data.m_name;
			case L0_Timestamp:
				return data.m_timestamp;
			case L0_ExperienceName:
				return data.m_experienceName;
			case L0_PlatformName:
				return data.m_platformName;
			case L0_SampleRate:
				return data.m_sampleRate;
			case L0_TimeLength:
				return data.m_timeLength;
			case L0_DataType:
				return data.m_datatype;
			case L0_DataSource:
				return data.m_datasource;
			default:
				break;
			}
		}

		return QVariant();
	}

	QVariant LevelModel0::customProcessRole(int role, int rowIndex, int dataIndex, int columnId) const
	{
		if (role == Qt::SizeHintRole) {
			return QSize(220,40);
		}
		return QVariant();
	}

	void LevelModel0::prepareData()
	{
		for (int i = 0; i < 100; i++) {
			Level0Data ld;
			ld.m_id = i + 1;
			ld.m_num = QString("%1").arg(100000 + i);
			ld.m_name = QString("Name_%1").arg(i + 1);
			ld.m_timestamp = QString("2021-01-10 12:00:0%1").arg(i);
			ld.m_experienceName = QStringLiteral("XXX实验");
			ld.m_platformName = QStringLiteral("XX平台%1").arg(i);
			ld.m_sampleRate = "44.1MHz";
			ld.m_timeLength = "120";
			ld.m_datatype = "mp3";
			ld.m_datasource = QStringLiteral("手动录入");
			
			m_dataList.append(ld);
		}

		refresh();
	}

	int LevelModel0::datasSize() const
	{
		return m_dataList.size();
	}

} //namespace Related 
