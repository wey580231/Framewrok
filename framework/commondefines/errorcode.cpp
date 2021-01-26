#include "errorcode.h"

namespace Datastruct {

	extern QString G_StrError(ErrorCode code)
	{
		switch (code)
		{
			case Datastruct::PASSWROD_ERROR:
				return QStringLiteral("账户密码错误!");
				break;
			case Datastruct::NO_REGIST:
				return QStringLiteral("账户未注册!");
				break;
			case Datastruct::USER_EXIST:
				return QStringLiteral("用户名已存在!");
				break;
			case Datastruct::NO_USER:
				return QStringLiteral("未找到指定用户!");
				break;
			case Datastruct::NO_PRIVILEGE:
				return QStringLiteral("无操作权限!");
				break;




			case Datastruct::SQL_EXECUTE_ERROR:
				return QStringLiteral("数据访问失败!");
				break;
			default:
				break;
		}

		return QStringLiteral("未知错误");
	}

}	//namespace Datastruct 
