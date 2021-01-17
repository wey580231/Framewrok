
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
		U_UserRights			/*!< 用户权限 */
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
	
} //namespace Related


#endif // DATASTRUCT_H
