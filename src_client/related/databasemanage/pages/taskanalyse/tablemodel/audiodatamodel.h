/*!
 * @brief     音频数据表格模型 
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 15:43:21
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

#include <QObject>
#include <base\selfwidget\tablemode\rtablemodel.h>
#include "../../../datastruct.h"

namespace Related {

	class AudioDataModel : public  Base::RTableModel
	{
		Q_OBJECT

	public:
		AudioDataModel(QObject *parent = nullptr);
		~AudioDataModel();

		void prepareData();

		int datasSize() const;

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<AudioData> m_dataList;

	};

}//namespace Related 