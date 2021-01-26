/*!
 * @brief     错误码  
 * @author    wey
 * @version   1.0
 * @date      2021.01.25 17:23:55
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

#include "commondefines_global.h"

namespace Datastruct {

	/*!
	 * @brief 数据访问、操作过程中错误码
	 * @details 客户端和服务器端通过统一的约定，减少硬编码
	 */
	enum ErrorCode {
		//业务相关
		PASSWROD_ERROR = 1,
		NO_REGIST,
		USER_EXIST,
		NO_USER,
		NO_PRIVILEGE,


		//数据库访问相关
		SQL_EXECUTE_ERROR = 100,
	};

	/*! 
	 * @brief 查询故障码对应的描述信息
	 * @param code 故障码
	 * @return 描述信息
	 */
	extern COMMONDEFINESHARED_EXPORT QString G_StrError(ErrorCode code);		/*!< 获取对应错误码描述信息 */

}	//namespace Datastruct 
