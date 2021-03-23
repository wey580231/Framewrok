/*!
 * @brief     数据库表字段映射
 * @details   定义各个数据库表以及表字段名,用于在访问数据库编写sql时，减少对字段的硬编码
 * @author    wey
 * @version   1.0
 * @date      2021.01.23 14:08:34
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>
#include <QString>

namespace Table {

	struct AbstractTable {
		AbstractTable(QString tname):table(tname) {}
		QString table;
	};

	/*!
	 * @brief 用户信息表
	 */
	struct UserEntity : public AbstractTable {
		UserEntity() :AbstractTable("user_entity") {}

		QString id = "id";
		QString userName = "user_name";
		QString userPassword = "user_password";
		QString regitstTime = "regist_time";
		QString lastLoadTime = "last_load_time";
		QString privilege = "privilege";
		QString superManage = "super_manager";
	};

	/*!
	 * @brief 侦测平台表
	 */
	struct DetectPlatformEntity : public AbstractTable {
		DetectPlatformEntity() :AbstractTable("detect_platform_entity") {}

		QString id = "id";
		QString name = "name";
	};

	/*!
	 * @brief 侦测平台亚型表
	 */
	struct DetectPlatformSubtypeEntity : public AbstractTable {
		DetectPlatformSubtypeEntity() :AbstractTable("detect_platform_subtype_entity") {}

		QString id = "id";
		QString detectId = "detect_id";
		QString name = "name";
	};

	/*!
	 * @brief 任务信息表
	 */
	struct TaskEntity : public AbstractTable {
		TaskEntity() :AbstractTable("task_entity") {}

		QString id = "id";
		QString name = "name";
		QString startTime = "start_time";
		QString endTime = "end_time";
		QString location = "location";
		QString lon = "lon";
		QString lat = "lat";
		QString description = "description";
	};

	/*!
	 * @brief 任务侦测平台亚型表
	 */
	struct TaskDetectPlatformEntity : public AbstractTable {
		TaskDetectPlatformEntity() :AbstractTable("task_detect_platform_entity") {}

		QString id = "id";
		QString taskId = "task_id";
		QString detectId = "detect_id";
		QString name = "name";
		QString sensorType = "sensor_type";
		QString platformPower = "platform_power";
		QString startTime = "start_time";
		QString endTime = "end_time";
		QString lastTime = "last_time";
	};

	/*!
	 * @brief 试验图片资源表
	 */
	struct TaskImageEntity : public AbstractTable {
		TaskImageEntity() :AbstractTable("task_image_resource") {}

		QString id = "id";
		QString taskId = "task_id";
		QString realName = "real_name";
		QString suffix = "suffix";
		QString uploadTime = "upload_time";
		QString imageSize = "image_size";
		QString md5 = "md5";
		QString savePath = "save_path";
		QString revcSize = "recv_size";
		QString description = "description";
	};

	/*!
	 * @brief 平台原始XML文件信息表
	 */
	struct TaskPlatformOriginalXmlEntity : public AbstractTable {
		TaskPlatformOriginalXmlEntity() :AbstractTable("platform_original_xml") {}

		QString id = "id";
		QString taskId = "task_id";
		QString detectPlatformId = "detect_platform_id";
		QString realName = "real_name";
		QString fileSize = "file_size";
		QString startTime = "start_time";
		QString endTime = "end_time";
		QString md5 = "md5";
		QString savePath = "save_path";
		QString revcSize = "recv_size";
	};

	/*!
	 * @brief 平台原始Data文件信息表
	 */
	struct TaskplatformOriginalDataEntity : public AbstractTable {
		TaskplatformOriginalDataEntity() :AbstractTable("platform_original_data") {}

		QString id = "id";
		QString taskId = "task_id";
		QString platformId = "platform_id";
		QString realName = "real_name";
		QString fileSize = "file_size";
		QString startFrameIndex = "start_frame_index";
		QString endFrameIndex = "end_frame_index";
		QString startTime = "start_time";
		QString endTime = "end_time";
		QString md5 = "md5";
		QString savePath = "save_path";
		QString revcSize = "recv_size";
	};

	/*!
	 * @brief 值班日志信息表
	 */
	struct DutyRecordEntity : public AbstractTable {
		DutyRecordEntity() :AbstractTable("task_duty_record") {}

		QString id = "id";
		QString taskId = "task_id";
		QString createTime = "create_time";
		QString description = "description";
		QString seaCondition = "sea_condition";
		QString wind = "wind";
		QString windSpeed = "wind_speed";
		QString waveHigh = "wave_high";
		QString oceanCurrents = "ocean_currents";
	};

	/*!
	 * @brief 试验记录信息表
	 */
	struct ExperimentRecordEntity : public AbstractTable {
		ExperimentRecordEntity() :AbstractTable("task_experiment_record") {}

		QString id = "id";
		QString taskId = "task_id";
		QString platformId = "platform_id";
		QString floatingTime = "floating_time";
		QString lon = "lon";
		QString lat = "lat";
		QString setHeadingDegree = "set_heading_degree";
		QString actualHeadingDegree = "actual_heading_degree";
		QString acousticState = "acoustic_state";
		QString targetNum = "target_num";
		QString underwaterTargetNum = "underwater_target_num";
		QString underwaterTargetInfo = "underwater_target_info";
		QString maxDepth = "max_depth";
		QString profileIndex = "profile_index";
		QString profileLength = "profile_length";
		QString profileDistance = "profile_distance";
	};

	/*!
	 * @brief 目标数据信息表
	 */
	struct  TargetDataEntity : public AbstractTable {
		TargetDataEntity() :AbstractTable("target_entity") {}

		QString id = "id";
		QString name = "name";
		QString type = "type";
		QString createTime = "create_time";
	};

	/*!
	 * @brief AIS数据信息表
	 */
	struct  AISDataEntity : public AbstractTable {
		AISDataEntity() :AbstractTable("ais_data") {}

		QString id = "id";
		QString targetId = "target_id";
		QString mmsi = "mmsi";
		QString time = "time";					
		QString lon = "lon";
		QString lat = "lat";
		QString course = "course";
		QString truehead = "truehead";
		QString name = "name";
		QString shipType = "ship_type";
		QString shipImo = "ship_imo";
		QString navStatus = "nav_status";
		QString speed = "speed";
		QString eta = "eta";
		QString dest = "dest";
		QString length = "length";
		QString width = "width";
		QString callsign = "callsign";
		QString flag = "flag";
		QString buildDate = "build_date";
		QString port = "port";
	};

}	//namespace Table
