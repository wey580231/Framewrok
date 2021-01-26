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

		const QString  offsetIndex = "offsetIndex";
		const QString  limitIndex = "limitIndex";

		const QString totalDataSize = "totalDataSize";

		const QString data = "data";

		const QString taskId = "taskId";
		const QString createTime = "createTime";

		const QString platformId = "platformId";

		const QString lon = "lon";

		const QString lat = "lat";

		const QString seaCondition = "seaCondition";

		const QString floatingTime = "floatingTime";

		const QString startTime = "startTime";

		const QString endTime = "endTime";

		const QString location = "location";

		const QString description = "description";

		const QString detectPlatform = "detectPlatform";
	};

} //namespace CommonDefines 