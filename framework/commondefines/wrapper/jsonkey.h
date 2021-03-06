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
		const QString lastLoadTime = "lastLoadTime";
		const QString privilege = "privilege";
		const QString manager = "manager";
		const QString result = "result";
		const QString errorInfo = "errorInfo";
		const QString offsetIndex = "offsetIndex";
		const QString limitIndex = "limitIndex";
		const QString totalDataSize = "totalDataSize";
		const QString type = "type";
		const QString manageId = "manageId";
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
		const QString wind = "wind";
		const QString windSpeed = "windSpeed";
		const QString waveHigh = "waveHigh";
		const QString oceanCurrents = "oceanCurrents";
		const QString setHeadingDegree = "setHeadingDegree";
		const QString actualHeadingDegree = "actualHeadingDegree";
		const QString acousticState = "acousticState";
		const QString targetNum = "targetNum";
		const QString underwaterTargetNum = "underwaterTargetNum";
		const QString underwaterTargetInfo = "underwaterTargetInfo";
		const QString maxDepth = "maxDepth";
		const QString profileIndex = "profileIndex";
		const QString profileLength = "profileLength";
		const QString profileDistance = "profileDistance";
		const QString detectId = "detectId";

		const QString count = "count";

		const QString targetId = "targetId";
		const QString mmsi = "mmsi";
		const QString time = "time";
		const QString course = "course";
		const QString truehead = "truehead";
		const QString shipType = "shipType";
		const QString shipImo = "shipImo";
		const QString navStatus = "navStatus";
		const QString speed = "speed";
		const QString eta = "eta";
		const QString dest = "dest";
		const QString length = "length";
		const QString width = "width";
		const QString callsign = "callsign";
		const QString flag = "flag";
		const QString buildDate = "buildDate";
		const QString port = "port";

		const QString realName = "realName";
		const QString suffix = "suffix";
		const QString uploadTime = "uploadTime";
		const QString imageSize = "imageSize";

		const QString sensorType = "sensorType";
		const QString platformPower = "platformPower";
		const QString lastTime = "lastTime";
	};

} //namespace CommonDefines 