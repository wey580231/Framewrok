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

#include <QMap>

#include "../related_global.h"

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
		Page_Setting,			/*!< 系统设置 */
		
		Page_TaskOverviewPage,	/*!< 任务概览页面 */
		Page_TaskRecordPage,	/*!< 任务记录页面 */
		Page_TaskDataAnalyse,	/*!< 任务数据分析 */	

		/*!< 子页面，按照‘Page_主页面_子页面’格式 */
		Page_Setting_UserManage,	/*!< 用户管理 */
		Page_Setting_SystemSetting	/*!< 系统设置 */
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
	 * @brief Level1表格列
	 */
	enum Level1ColumnIndex {
		L1_Id,
		L1_Num,
		L1_Name,
		L1_Timestamp,
		L1_PlatformName,
		L1_TimeLength,
		L1_DataType
	};

	/*!
	 * @brief Level1数据结构
	 */
	struct Level1Data {
		int m_id;
		QString m_num;					/*!< 批号 */
		QString m_name;
		QString m_timestamp;
		QString m_platformName;			/*!< 平台名 */
		QString m_timeLength;			/*!< 采样时长 */
		QString m_datatype;				/*!< 文件格式 */
	};

	/*!
	 * @brief Level2表格列
	 */
	enum Level2ColumnIndex {
		L2_Id,
		L2_Num,					/*!< 批号 */
		L2_Timestamp,			
		L2_Nation,				/*!< 国籍 */
		L2_Type,				/*!< 类型 */
		L2_JyNum,				/*!< 桨叶数 */
		L2_ZxDegree,			/*!< 置信度 */
		L2_Location				/*!< 录取地点 */
	};

	/*!
	 * @brief Level2数据结构
	 */
	struct Level2Data {
		int m_id;
		QString m_num;
		QString m_timestamp;
		QString m_nation;
		QString m_type;
		QString m_jyNum;
		QString m_zxDegree;
		QString m_location;
	};

	/*!
	 * @brief 数据分析页面数据结构
	 */
	enum DataAnalyseColumnIndex {
		D_Id,
		D_BatchNumber,
		D_Time,
		D_LocalDistinguish,
		D_Result,
		D_PlatformName,
		D_DataType
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
	 * @brief 
	 */
	enum  TargetDatabaseDataColumnIndex {
		T_Index,					/*!< 索引 */
		T_TargetName,				/*!< 目标名称 */
		T_Edttime,					/*!< 录取时间 */
		T_Tonnage,					/*!< 吨位 */
		T_AxlesNumber,				/*!< 轴数 */
		T_Datalength,				/*!< 数据时长 */
		T_Type						/*!< 类型 */
	};

	struct TargetDatabaseData{
		int  index;					/*!< 索引 */
		QString targetName;			/*!< 目标名称 */
		QString edttime;			/*!< 录取时间 */
		double tonnage;				/*!< 吨位 */
		int  axlesNumber;			/*!< 轴数 */
		int datalength;				/*!< 数据时长 */
		int type;					/*!< 类型 */
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
		TaskBaseInfo() :lon (0), lat(0){

		}
		QString  taskName;				/*!< 任务名称 */
		QString  taskLocation;			/*!< 任务地点 */
		QString  startTime;				/*!< 起始时间 */
		QString  endTime;				/*!< 结束时间 */
		double lon;						/*!< 经度 */
		double lat;						/*!< 纬度 */
		QString taskDescription;		/*!< 描述 */
	};

	/*!
	 * @brief   原始数据文件参数信息
	 * @details 
	 */
	struct OriginalDataFileParameter
	{
		QString name;				/*!< 名称 */
		QString path;				/*!< 路径: 相对路径 */
		double size;				/*!< 文件大小 */
		int startIndex;				/*!< 起始数据帧 */
		int  endIndex;				/*!< 结束数据帧 */
		QString createTime;			/*!< 创建时间 */
		bool isDir;					/*!< 是否为文件夹 */
	};


	/*!
	* @brief 值班日志表格列索引
	*/
	enum  LogbookDataColumnIndex {
		L_Index,					/*!< 索引 */
		L_TaskId,					/*!< 任务Id */
		L_CreateTime,				/*!< 录入时间 */
		L_Description,				/*!< 描述 */
		L_SeaCondition,				/*!< 海况信息 */
	};
	
	/*!
	* @brief 试验记录表格列索引
	*/
	enum TrialSheetDataColumnIndex
	{
		TS_Index,					/*!< 索引 */
		TS_TaskId,					/*!< 任务Id */
		TS_PlatformId,				/*!< 平台Id */
		TS_Lon,						/*!< 经度 */
		TS_Lat,						/*!< 纬度 */
		TS_SeaCondition,			/*!< 海况 */
		TS_FloatingTime,			/*!< 浮动时间 */
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

} //namespace Related


#endif // DATASTRUCT_H
