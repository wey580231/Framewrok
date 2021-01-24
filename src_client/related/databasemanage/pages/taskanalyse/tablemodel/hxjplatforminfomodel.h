/*!
 * @brief     HXJ平台信息数据表格模型
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.01.23 16:35:04
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

#include <base\selfwidget\tablemode\rtablemodel.h>

#include "../../../datastruct.h"

namespace Related {

	class HXJPlatformInfoModel : public  Base::RTableModel
	{
		Q_OBJECT

	public:
		HXJPlatformInfoModel(QObject *parent = nullptr);
		~HXJPlatformInfoModel();

		void prepareData();

		int datasSize() const;

	protected:
		QVariant displayData(int rowIndex, int dataIndex, int columnId) const;

	private:
		QList<HXJPlatformInfoData> m_dataList;

	};

}//namespace Related 
