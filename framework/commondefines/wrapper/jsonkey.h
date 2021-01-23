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

		const QString name = "name";
		const QString password = "password";

		const QString result = "result";
		const QString errorInfo = "errorinfo";

	};

} //namespace CommonDefines 