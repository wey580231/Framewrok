/*!
 * @brief     2级表格数据模型 
 * @author    wey
 * @version   1.0
 * @date      2021.01.10 13:53:43
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QObject>

#include <base\selfwidget\tablemode\rtablemodel.h>
#include "../../../datastruct.h"

namespace Related {

	class LevelModel2 : public Base::RTableModel
	{
		Q_OBJECT

	public:
		LevelModel2(QObject *parent = nullptr);
		~LevelModel2();

		void prepareData();
		int datasSize() const;

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;
		QVariant customProcessRole(int role, int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<Level2Data> m_dataList;
	};

} //namespace Related 
