#include "levelmodel2.h"

namespace Related {

	LevelModel2::LevelModel2(QObject *parent)
		: Base::RTableModel(parent)
	{

	}

	LevelModel2::~LevelModel2()
	{
	}

	QVariant LevelModel2::displayData(int rowIndex, int dataIndex, int columnId) const
	{
		Level0ColumnIndex cindex = static_cast<Level0ColumnIndex>(columnId);
		if (dataIndex >= 0 && dataIndex < m_dataList.size()) {
			Level2Data data = m_dataList.at(dataIndex);

			switch (cindex)
			{
			case L2_Id:
				return data.m_id;
			case L2_Num:
				return data.m_num;
			case L2_Timestamp:
				return data.m_timestamp;
			case L2_Nation:
				return data.m_nation;
			case L2_Type:
				return data.m_type;
			case L2_JyNum:
				return data.m_jyNum;
			case L2_ZxDegree:
				return data.m_zxDegree;
			case L2_Location:
				return data.m_location;
			default:
				break;
			}
		}

		return QVariant();
	}

	QVariant LevelModel2::customProcessRole(int role, int rowIndex, int dataIndex, int columnId) const
	{
		if (role == Qt::SizeHintRole) {
			return QSize(220,40);
		}
		return QVariant();
	}

	void LevelModel2::prepareData()
	{
		for (int i = 0; i < 100; i++) {
			Level2Data ld;
			ld.m_id = i + 1;
			ld.m_num = QString("%1").arg(100000 + i);
			ld.m_timestamp = QString("2021-01-10 12:00:0%1").arg(i);
			ld.m_nation = QStringLiteral("美国");
			ld.m_type = QStringLiteral("水面舰/XX级/XX号");
			ld.m_jyNum = QString("4");
			ld.m_zxDegree = QString("85");
			ld.m_location = QStringLiteral("120.123456E,25.123456N");
			m_dataList.append(ld);
		}

		refresh();
	}

	int LevelModel2::datasSize() const
	{
		return m_dataList.size();
	}

} //namespace Related 
