
/*!
 *  @brief	   数据定义
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2019.05.20
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef DATASTRUCT_H_2019_05_20
#define DATASTRUCT_H_2019_05_20

#include <commondefines/datastruct.h>
#include "../related_global.h"

namespace Related {
	
	enum PageType {
		Page_MainPage,
		Page_DataManage,
		Page_DataAnalyse,
		Page_Setting
	};

	enum Level0ColumnIndex {
		C_Id,
		C_Num,
		C_Timestamp,
		C_ExperienceName,
		C_PlatformName,
		C_TimeLength,
		C_DataType
	};

	struct Level0Data {
		int m_id;
		QString m_name;
		QString m_timestamp;
	};


} //namespace Related


#endif // DATASTRUCT_H
