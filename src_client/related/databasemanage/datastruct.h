﻿/*!
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

#include <QMap>

#include "../related_global.h"
#include "global.h"

#define TIME_FORMAT "yyyy-MM-dd hh:mm:ss"

namespace Related {

	/*!
	 * @brief 窗口整体布局视图类型
	 * @details 
	 */
	enum ViewModel {
		SystemView,		/*!< 系统级视图 */
		TaskView		/*!< 任务级视图 */
	};
	
	/*!
	 * @brief 左侧菜单页面类型
	 * @details 用于关联左侧菜单项目和右侧页面
	 */
	enum PageType {
		/*!< 主要页面，用于左侧菜单栏切换 */
		Page_SystemMainPage,	/*!< 系统主页 */
		Page_TargetDatabase,	/*!< 系统目标库 */
		Page_AisDataBase,		/*!< AIS数据库 */
		Page_Setting,			/*!< 系统设置 */
		
		Page_TaskOverviewPage,	/*!< 任务概览页面 */
		Page_TaskRecordPage,	/*!< 任务记录页面 */
		Page_TaskDataAnalyse,	/*!< 任务数据分析 */	

		/*!< 子页面，按照‘Page_主页面_子页面’格式 */
		Page_TargetDatabase_TargetOverview,		/*!< 目标概览页面 */
		Page_TargetDatabase_TargetDetail,		/*!< 单个目标详情 */

		Page_Setting_UserManage,				/*!< 用户管理 */
		Page_Setting_SystemSetting,				/*!< 系统设置 */
		Page_Setting_DetectPlatformPage,		/*!< 侦测平台 */	

		Page_TaskRecord_DutyRecord,				/*!< 值班日志 */
		Page_TaskRecord_ExperimentRecord,		/*!< 试验记录 */
		Page_TaskRecord_TaskReport,				/*!< 任务报告 */

		Page_TaskAnalyse_DataOverview,			/*!< 数据预览 */
		Page_TaskAnalyse_TimeDomainAnalysis,	/*!< 时域分析 */
		Page_TaskAnalyse_FrequencyDomainAnalysis,	/*!< 频域分析 */
		Page_TaskAnalyse_DomenAnalysis,			/*!< Demen分析 */
		Page_TaskAnalyse_LofarAnalysis,			/*!< Lofar分析 */
		Page_TaskAnalyse_SituationAnalysi,		/*!< 态势分析 */

		Page_AisDataBase_AisData				/*!< AIS数据 */
	};

	/*!
	 * @brief Level0表格列
	 */
	enum Level0ColumnIndex {
		L0_Id,
		L0_Num,					/*!< 批号 */
		L0_Name,				/*!< 文件名 */
		L0_Timestamp,			/*!< 时间戳 */
		L0_ExperienceName,		/*!< 实验名称 */
		L0_PlatformName,		/*!< 平台名 */
		L0_SampleRate,			/*!< 采样率 */
		L0_TimeLength,			/*!< 时长 */
		L0_DataType,			/*!< 文件格式 */
		L0_DataSource			/*!< 数据源来源 */
	};

	/*!
	 * @brief Level0数据结构
	 */
	struct Level0Data {
		int m_id;
		QString m_num;					/*!< 批号 */
		QString m_name;					/*!< 文件名 */
		QString m_timestamp;			/*!< 时间戳 */
		QString m_experienceName;		/*!< 实验名称 */
		QString m_platformName;			/*!< 平台名 */
		QString m_sampleRate;			/*!< 采样率 */
		QString m_timeLength;			/*!< 采样时长 */
		QString m_datatype;				/*!< 文件格式 */
		QString m_datasource;			/*!< 数据源来源 */
	};

	/*!
	 * @brief 用户数据管理结构
	 */
	struct UserManageData {
		int m_id;
		QString m_name;				/*!< 用户名 */
		QString m_timestamp;		/*!< 注册时间 */
		QString m_loginTimestamp;	/*!< 上次登录时间 */
		QString m_rights;			/*!< 用户权限 */
	};


	/*!
	 * @brief 平台描述信息
	 */
	struct platformInfo {
		QString path;

	};

	typedef  QList<platformInfo> PlatformInfos;
	/*!
	 * @brief 任务基本信息
	 */
	struct  TaskBaseInfo {
		TaskBaseInfo(){

		}
		QString taskName;				/*!< 任务名称 */
		QString taskLocation;			/*!< 任务地点 */
		QString startTime;				/*!< 起始时间 */
		QString endTime;				/*!< 结束时间 */
		QString lon;					/*!< 经度 */
		QString lat;					/*!< 纬度 */
		QString taskDescription;		/*!< 描述 */
	};


	/*!
	 * @brief 用户管理表格列
	 */
	enum UserManageColumnIndex {
		U_Id,
		U_UserName,				/*!< 用户名 */
		U_RegisterTime,			/*!< 注册时间 */
		U_LastLoginTime,		/*!< 上次登录时间 */
		U_UserRights,			/*!< 用户权限 */
		U_IsManage				/*!< 是否为管理员 */
	};

	/*!
	 * @brief 文件描述表格列
	 */
	enum FileDescriptionTableColumn {
		FileDescription_Index,					/*!< 索引 */
		FileDescription_Name,					/*!< 文件名 */
		FileDescription_FileTimeStamp,			/*!< 文件创建时间 */
		FileDescription_FileType,				/*!< 文件类型 */
		FileDescription_FileSize,				/*!< 文件大小 */
	};

	enum  FileType{
		File_image,
		File_Xml,
		File_Dat,
	};

	/*!
	 * @brief  文件描述书
	 * @details 文件基本的描述信息
	 */
	struct FileDescriptionData {
		FileDescriptionData() : m_size(0){
		}

		QString m_name;					/*!< 名称 */
		QString m_suffix;				/*!< 后缀名 */
		qint64 m_size;					/*!< 大小 */
		QString m_fileTimeStamp;		/*!< 时间 */
		QString m_md5;					/*!< MD5 */
		QString m_filePath;				/*!< 全路径 */
		QString m_rootPath;				/*!< 根路径 */

		QString m_taskId;				/*!< 任务Id */
		FileType m_FileType;			/*!< 文件类型 */
	};


	/*!
	* @brief 值班日志表格列索引
	*/
	enum  DutyRecordDataColumnIndex {
		DR_Index,						/*!< 索引 */
		DR_TaskId,						/*!< 任务Id */
		DR_CreateTime,					/*!< 录入时间 */
		DR_Description,					/*!< 描述 */
		DR_SeaCondition,				/*!< 海况 */
		DR_Wind,						/*!< 风向 */
		DR_WindSpeed,					/*!< 风速 */
		DR_WaveHigh,					/*!< 浪高 */
		DR_OceanCurrents,				/*!< 洋流 */
	};
	
	/*!
	* @brief 试验记录表格列索引
	*/
	enum ExperimentRecordDataColumnIndex{
		ER_Index,						/*!< 索引 */
		ER_TaskId,						/*!< 任务标识 */
		ER_PlatformId,					/*!< 平台标识 */
		ER_FloatingTime,				/*!< 上浮时间 */
		ER_Lon,							/*!< 上浮经度 */
		ER_Lat,							/*!< 上浮纬度 */
		ER_SetHeadingDegree,			/*!< 设置航向角 */
		ER_ActualHeadingDegree,			/*!< 实际航向角 */
		ER_AcousticState,				/*!< 声学系统状态 */
		ER_TargetNum,					/*!< 目标总数 */
		ER_UnderwaterTargetNum,			/*!< 水下目标数 */
		ER_UnderwaterTargetInfo,		/*!< 水下目标信息 */
		ER_MaxDepth,					/*!< 剖面最大深度 */
		ER_ProfileIndex,				/*!< 剖面序号 */
		ER_ProfileLength,				/*!< 剖面时长 */
		ER_ProfileDistance,				/*!< 剖面移动距离 */
	};

	enum DetectPlatformSubtypeIndex {
		DPS_Index,
	};

	/*!
	* @brief 数据预览表格列
	*/
	enum  DataOverviewColumnIndex
	{
		DO_Index,					/*!< 索引 */
		DO_FileName,				/*!< 文件名 */
		DO_PlatformName,			/*!< 平台名称*/
		DO_PlatformType,			/*!< 平台类型*/
		Do_DataSize,
		DO_StartIndex,
		DO_EndIndex,
	};

	/*!
	* @brief 数据预览数据
	*/
	struct DataOverviewData
	{
		int index;					/*!< 索引 */
		QString fileName;			/*!< 文件名 */
		QString platformName;		/*!< 平台名称 */
		int platformType;			/*!< 平台类型 */
		double dataSize;			/*!< 数据长度 */
		int startIndex;				/*!< 起始帧 */
		int endIndex;				/*!< 结束帧 */
	}; 

	/*!
	* @brief 音频数据表格列
	*/
	enum AudioDataColumnIndex {
		AD_Index,					/*!< 索引 */				
		AD_Name,					/*!< 文件名 */
		AD_TimeLength,				/*!< 时长 */
		AD_PlatformName,			/*!< 平台名 */
		AD_DataSize,				/*!< 数据大小 */
	};

	/*!
	* @brief 音频数据信息
	*/
	struct AudioData
	{
		int index;					/*!< 索引 */
		QString name;				/*!< 文件名 */
		int timeLength;				/*!< 时长 */
		QString platformName;		/*!< 平台名称 */
		double dataSize;			/*!< 数据长度 */
	};

	/*!
	 * @brief  环境噪声表格列
	 * @details 
	 */
	enum AmbientNoiseColumnIndex{
		AN_Index,					/*!< 索引 */
		AN_FileName,				/*!< 文件名称 */
		AN_PlatformName,			/*!< 平台名称 */
		AN_TimeLength,				/*!< 时长 */
		AN_DataSize,				/*!< 数据大小 */
		AN_StartIndex,				/*!< 起始索引 */
		AN_EndIndex,				/*!< 结束索引 */
	};

	/*!
	 * @brief	环境噪声数据
	 * @details 
	 */
	struct AmbientNoiseData{
		int  index;					/*!< 索引 */
		QString fileName;			/*!< 文件名称 */
		QString platformName;		/*!< 平台名称 */
		int timeLength;				/*!< 时长 */
		int dataSize;				/*!< 数据大小 */
		int startIndex;				/*!< 起始索引 */
		int endIndex;				/*!< 结束索引 */
	};
	
	/*!
	 * @brief  HXJ平台信息数据表格列
	 * @details 
	 */
	enum HXJPlatformInfoColumnIndex {
		HXJPI_Index,				/*!< 索引 */
		HXJPI_PlatformName,			/*!< 平台名称 */
		HXJPI_DataIndex,			/*!< 数据帧索引 */
		HXJPI_InputTime,			/*!< 录入时间 */
		HXJPI_CourseAngle,			/*!< 航向角 */
		HXJPI_PitchAngle,			/*!< 俯仰角 */
		HXJPI_RollAngle,			/*!< 横滚角 */
	};

	/*!
	 * @brief	HXJ平台信息数据
	 * @details
	 */
	struct HXJPlatformInfoData {
		int  index;					/*!< 索引 */
		QString platformName;		/*!< 平台名称 */
		int dataIndex;				/*!< 数据帧索引 */
		QString inputTime;			/*!< 录入时间 */
		int courseAngle;			/*!< 航向角 */
		int pitchAngle;				/*!< 俯仰角 */
		int rollAngle;				/*!< 横滚角 */
	};


	/*!
	 * @brief 目标数据表格列
	 */
	enum  TargetDataColumnIndex {
		T_Index,					/*!< 索引 */
		T_TargetName,				/*!< 目标名称 */
		T_Edttime,					/*!< 录取时间 */
		T_Tonnage,					/*!< 吨位 */
		T_Lon,						/*!< 经度 */
		T_Lat,						/*!< 纬度 */
		T_Speed,					/*!< 航行速度 */
		T_AxlesNumber,				/*!< 轴数 */
		T_Datalength,				/*!< 数据时长 */
		T_Type						/*!< 类型 */
	};

	/*!
 * @brief 目标数据表格列
 */
	enum  TargetWavDataColumnIndex {
		TWAV_Index,					/*!< 索引 */
		TWAV_TargetName,				/*!< 目标名称 */
		TWAV_Edttime,					/*!< 录取时间 */
		TWAV_Tonnage,					/*!< 吨位 */
		TWAV_Lon,						/*!< 经度 */
		TWAV_Lat,						/*!< 纬度 */
		TWAV_Speed,					/*!< 航行速度 */
		TWAV_AxlesNumber,				/*!< 轴数 */
		TWAV_Datalength,				/*!< 数据时长 */
		TWAV_Type						/*!< 类型 */
	};

	/*!
	 * @brief AIS数据表格列
     */
	enum  AISDATAColumnIndex {
		AIS_Index,					/*!< 唯一标识 */
		AIS_TargetId,				/*!< 目标标识 */
		AIS_Mmsi,					/*!< 船舶MMSI */
		AIS_Time,					/*!< 实时信号时间 */
		AIS_Lon,					/*!< 经度 */
		AIS_Lat,					/*!< 纬度 */
		AIS_Course,					/*!< 航迹向 */
		AIS_Truehead,				/*!< 航首向 */
		AIS_Name,					/*!< 船名 */
		AIS_ShipType,				/*!< 船舶以及货物类型 */
		AIS_NavStatus,				/*!< 航行状态 */
		AIS_Speed,					/*!< 航行速度 */
		AIS_Length,					/*!< 船长 */
		AIS_Width,					/*!< 船宽 */
		AIS_BuildDate,				/*!< 建造时间 */
		AIS_Port,					/*!< 船籍港 */
	};



} //namespace Related


#endif // DATASTRUCT_H
