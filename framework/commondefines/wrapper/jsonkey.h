/*!
 * @brief     集中定义JSON中key名 
 * @author    wey
 * @version   1.0
 * @date      2021.01.23 09:07:37
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>
#include <QString>

namespace CommonDefines {

	class JsonKey
	{

	public:
		JsonKey();
		~JsonKey();

		const QString id = "id";
		const QString name = "name";
		const QString password = "password";
		const QString registTime = "registTime";
		const QString privilege = "privilege";
		const QString manager = "manager";

		const QString result = "result";
		const QString errorInfo = "errorinfo";

		const QString data = "data";
	};

} //namespace CommonDefines 