/*!
 * @brief     客户端和服务器端通信数据协议
 * @attention
			  1.若由客户端发送至服务器，则统一以Request结尾；
			  2.若由服务器发回客户端，则统一以Response结尾；
 * @author    wey
 * @version   1.0
 * @date      2021.01.20 15:45:05
 * @warning	  客户端和服务器端的版本必须一致
 * @copyright NanJing RenGu.
 * @note
 */
#ifndef DATASTRUCT_H_2020_06_01
#define DATASTRUCT_H_2020_06_01

#include <QtGlobal>
#include <QVariant>

#include "structdefines.h"
#include "errorcode.h"

namespace Datastruct {

#pragma pack(push,1)

#define PACK_HEAD 0xAABB7788		/*!< 报文头头标识 */
#define PACK_TAIL 0x8877BBAA		/*!< 报文尾标识 */

	/*!
	 * @brief 通讯数据包报文类型
	 * @details 1.客户端和服务器端通过此类型进行识别数据包，确保两者统一
				2.同一种包类型，客户端请求时用于标识向服务器端发送数据，当客户端接收到此种类型数据则表明是请求的响应。
				3.在绝大部分情况下，两者是发送和接收是一一对应的关系。
	 */
	enum PacketType {
		P_UserLogin = 1,					/*!< 用户登录 */
		P_UserRegist,						/*!< 用户注册 */
		P_UserList,							/*!< 用户列表查询 */
		P_UserOperate,						/*!< 用户信息更新,包括权限更新、删除用户 */

		P_CreateTask = 5,					/*!< 创建任务 */
		P_TaskList,							/*!< 查询所有任务 */
		P_TaskByCondition,					/*!< 按条件查询任务 */
		P_TaskDelete,						/*!< 删除任务 */
		P_TaskStaticsInfo,					/*!< 任务统计信息，包括任务数、占用空间大小等 */
		P_TaskSimpleInfo,					/*!< 单个任务概览信息 */
		P_TaskFullInfo,						/*!< 单个任务详细信息 */
		P_TaskModify,						/*!< 单个任务信息修改 */

		P_TaskDetectPlatformCreate,			/*!< 任务侦测平台亚型创建 */
		P_TaskDetectPlatformList,			/*!< 查询指定任务侦测平台亚型 */
		P_TaskDetectPlatformByCondition,	/*!< 按条件查询侦测平台亚型 */
		P_TaskDetectPlatformDelete,			/*!< 任务侦测平台亚型删除 */
		P_TaskDetectPlatformModify,			/*!< 任务侦测平台亚型修改 */

		P_TaskDataFileCreate,				/*!< 任务数据文件创建 */
		P_TaskDataFileDelete,				/*!< 任务数据文件删除 */

		P_TaskImageList,					/*!< 查询任务试验图片资源 */
		P_TaskImageByCondition,				/*!< 按条件查询任务试验图片资源 */
		P_TaskImageDelete,					/*!< 任务试验图片资源删除 */
		P_TaskImageModify,					/*!< 任务试验图片资源修改 */

		P_CreateDutyRecord,					/*!< 创建值班日志 */
		P_ListDutyRecords ,					/*!< 查询所有值班日志 */
		P_DutyRecordByCondition,			/*!< 按条件查询值班日志 */
		P_DeleteDutyRecords,				/*!< 删除值班日志-单条 */
		P_ModifyDutyRecord,					/*!< 修改值班日志 */

		P_CreateExperimentRecord,			/*!< 创建试验记录 */
		P_ListExperimentRecords,			/*!< 查询所有试验记录 */
		P_ExperimentRecordByCondition,		/*!< 按条件查询试验记录 */
		P_DeleteExperimentRecord,			/*!< 删除试验记录 - 单条*/
		P_ModifyExperimentRecord,			/*!< 修改试验记录 */

		P_DetectPlatformCreate = 40,		/*!< 创建侦测平台 */
		P_DetectPlatformList,				/*!< 查询所有侦测平台 */
		P_DetectPlatformDelete,				/*!< 删除侦测平台 */
		P_DetectPlatformModify,				/*!< 修改侦测平台 */

		P_DetectPlatformSubtypeCreate = 45,	/*!< 创建侦测平台亚型 */
		P_DetectPlatformSubtypeList,		/*!< 查询所有侦测平台亚型 */
		P_DetectPlatformSubtypeDelete,		/*!< 删除侦测平台亚型 */
		P_DetectPlatformSubtypeModify,		/*!< 修改侦测平台亚型 */

		//目标信息
		P_TargetCreate = 50,				/*!< 创建目标 */
		P_TargetList,						/*!< 查询所有目标 */
		P_TargetByCondition,				/*!< 按条件查询目标 */
		P_TargetDelete,						/*!< 删除目标 */
		P_TargetModify,						/*!< 修改目标 */
		P_TargetSimpleInfo,					/*!< 单个目标概览信息 */
		P_TargetFullInfo,					/*!< 单个目标详细信息 */

		//AIS信息
		P_AISCreate = 60,					/*!< 创建AIS */
		P_AISList,							/*!< 查询所有AIS */
		P_AISByCondition,					/*!< 按条件查询AIS */
		P_AISDelete,						/*!< 删除AIS */
		P_AISModify,						/*!< 修改AIS */
		P_AISSimpleInfo,					/*!< 单个AIS概览信息 */
		P_AISFullInfo,						/*!< 单个AIS详细信息 */

		//原始文件
		P_FileData,

	};

	/*!
	 * @brief 通讯数据协议头
	 * @details 用于标识数据包类型，判断数据包边界
	 */
	struct PacketHead {
		PacketHead() :m_magicHead(PACK_HEAD), m_dataLen(0), m_packetType(0) {}
		uint m_magicHead;		/*!< 报文头标识 */
		uint m_dataLen;			/*!< 包含数据头、数据区、数据尾长度 */
		ushort m_packetType;	/*!< 指令类型 */
	};

	struct PacketTail {
		PacketTail() :m_magicTail(PACK_TAIL) {}
		uint m_magicTail;		/*!< 报文尾标识 */
	};


	/*!
	 * @brief  任务原始数据请求
	 * @details
	 */
	struct FileInfoParameter {
		FileInfoParameter() : m_totalLength(0), m_currentLength(0) {
			memset(m_fileId, 0, 32);
		}

		char m_fileId[32];
		qint64 m_totalLength;
		qint64 m_currentLength;
	};

#pragma  pack(pop)

	/*!
	 * @brief 用户登录请求
	 */
	struct UserLoginRequest {
		
		QString m_name;			/*!< 用户名 */
		QString m_password;		/*!< 密码，转换成MD5 */
	};

	/*!
	 * @brief 用户登录结果报文
	 */
	struct UserLoginResponse {
		UserLoginResponse() :m_loginResult(false) { }

		bool m_loginResult;		/*!< 登录结果信息，true:登录成功，false:登录失败 */
		ErrorCode m_errorCode;	/*!< 登录失败时说明失败原因 */

		UserEntityData m_userInfo;	/*!< 登录成功后，返回基本信息；失败不返回 */
	};

	/*!
	 * @brief 用户注册请求
	 */
	struct UserRegistRequest {

		QString m_name;			/*!< 用户名 */
		QString m_password;		/*!< 密码，转换成MD5 */
	};

	/*!
	 * @brief 用户登录结果报文
	 */
	struct UserRegistResponse {
		UserRegistResponse() : m_loginResult(false) {}

		bool m_loginResult;		/*!< 注册结果，true:注册成功，false:注册失败 */
		ErrorCode m_errorCode;	/*!< 注册失败时说明失败原因 */
	};

	/*!
	 * @brief 加载所有用户请求
	 */
	struct LoadAllUserRequest {

		QString m_name;			/*!< 当前操作用户名 */
		int m_offsetIndex;		/*!< 分页时，需加载的起始页偏移量 */
		int m_limitIndex;		/*!< 当前页面显示条数 */
	};

	/*!
	 * @brief 加载所有用户结果报文
	 */
	struct LoadAllUserResponse {
		LoadAllUserResponse() :m_userCount(0) {}

		int m_userCount;		/*!< 用户总条数 */
		QList<UserEntityData> m_userInfos;		/*!< 当前页面下用户结果集合 */
	};

	/*!
	 * @brief 用户基础操作类型
	 */
	enum UserOperateType {

		UpdateInfo,			/*!< 更新信息 */
		EditPrivilege,		/*!< 编辑用户权力 */
		DeleteUser			/*!< 删除用户 */
	};

	/*!
	 * @brief 用户基础操作请求
	 * @details 包括更新权限、删除用户等
				普通用户可以更新密码；
				管理员可以更新自己密码，修改普通用户的权限、删除普通用户
	 */
	struct OperateUserRequest {
		OperateUserRequest() :m_manageId(0), m_id(0) {}

		UserOperateType m_operateType;	/*!< 用户操作类型 */

		/*!< 普通用户操作内容 */
		int m_id;					/*!< 待操作的用户ID */
		QString m_password;			/*!< 新数据库密码 */

		/*!< 管理员用户操作内容 */
		int m_privilege;			/*!< 权限 */
		bool m_isManage;			/*!< 是否为管理员 */

		int m_manageId;				/*!< 管理员Id */
	};

	/*!
	 * @brief 用户基础操作响应
	 */
	struct OperateUserResponse {

		UserOperateType m_operateType;
		bool m_operateResult;		/*!< 操作结果信息，true:操作成功，false:操作失败 */
		ErrorCode m_errorCode;		/*!< 操作失败时说明失败原因 */
	};

	/*!
	 * @brief 创建任务请求
	 */
	struct  TaskCreateRequest{
		TaskCreateRequest() {
		}

		QString m_taskId;				/*!< 数据库Id */
		QString m_taskName;				/*!< 任务名称 */
		QString m_startTime;			/*!< 起始时间 */
		QString m_endTime;				/*!< 结束时间 */
		QString m_location;				/*!< 任务地点 */
		QString lon;					/*!< 经度 */
		QString lat;					/*!< 纬度 */
		QString description;			/*!< 描述 */
	};

	/*!
	 * @brief 任务创建响应
	 */
	struct TaskCreateResponse {
		TaskCreateResponse() : m_createResult(false) {
		}

		bool m_createResult;			/*!< 注册结果，true:注册成功，false:注册失败 */
		ErrorCode m_errorInfo;			/*!< 注册失败时说明失败原因 */
		TaskEntityData taskInfo;
	};

	/*!
	 * @brief  加载所任务请求
	 */
	struct  LoadAllTaskRequest {
		LoadAllTaskRequest():m_offsetIndex(0), m_limitIndex(0){
		}

		int m_offsetIndex;							/*!< 分页时，需加载的起始页偏移量 */
		int m_limitIndex;							/*!< 当前页面显示条数 */
	};

	/*!
	 * @brief  加载所有任务响应
	 */
	struct LoadAllTaskResponse {
		LoadAllTaskResponse() :m_count(0){
		}

		int m_count;									/*!< 任务总条数 */
		QList<TaskEntityData> m_taskInfos;				/*!< 当前页面下任务结果集合 */
	};

	/*!
	 * @brief   按条件查询任务请求
	 */
	struct TaskByConditionRequest {

		QString startTime;							/*!< 起始时间 */
		QString endTime;							/*!< 结束时间 */
		QString location;							/*!< 任务地点 */
	};

	/*!
	 * @brief   按条件查询任务响应
	 */
	struct TaskByConditionResponse {
		TaskByConditionResponse() : m_count(0){
		}

		int m_count;									/*!< 任务总条数 */
		QList<TaskEntityData> m_taskInfos;				/*!< 当前页面下任务结果集合 */
	};

	/*!
	 * @brief   任务删除请求
	 */
	struct TaskDeleteRequest {

		QString taskId;				/*!< 任务Id */
	};

	/*!
	 * @brief  任务删除请求结果报文
	 */
	struct TaskDeleteResponse {
		TaskDeleteResponse() : m_deleteResult(false) {
		}

		bool m_deleteResult;					/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;					/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief  任务统计信息请求
	 */
	struct  TaskStaticsInfoRequest{

		QString id;
	};

	/*!
	 * @brief  任务统计信息响应
	 */
	struct  TaskStaticsInfoResponse {

		QString  allTaskStartTime;								/*!< 所有任务的起始时间 */
		QString  allTaskEndTime;								/*!< 所有任务的结束时间 */
		QList<QString> listAlllocations;						/*!< 统计海区信息 */
	};

	/*! 
	 * @brief  单条任务预览信息请求
	 */
	struct TaskSimpleRequest{

		QString taskId;				/*!< 任务Id */
	};

	/*!
	 * @brief  单条任务预览信息响应
	 */
	struct TaskSimpleResponse {
		TaskSimpleResponse() : m_result(false) {
		}

		bool m_result;					/*!< 注册结果，true:注册成功，false:注册失败 */
		ErrorCode m_errorInfo;			/*!< 注册失败时说明失败原因 */
		TaskEntityData taskInfo;
	};

	/*!
	 * @brief    单个任务详细信息
	 */
	struct TaskFullInfoRequest {

	};

	/*!
	 * @brief    单个任务详细信息
     */
	struct TaskFullInfoResponse {

	};

	/*!
	 * @brief  单条任务修改信息请求
	 */
	struct TaskModifyRequest {

		QString taskId;				/*!< 数据库Id */
		QString taskName;			/*!< 任务名称 */
		QString startTime;			/*!< 起始时间 */
		QString endTime;			/*!< 结束时间 */
		QString location;			/*!< 任务地点 */
		QString lon;				/*!< 经度 */
		QString lat;				/*!< 纬度 */
		QString description;		/*!< 描述 */
	};

	/*!
	 * @brief  单条任务修改信息响应
	 */
	struct TaskModifyResponse {
		TaskModifyResponse() : m_result(false) {
		}

		bool m_result;					/*!< 注册结果，true:注册成功，false:注册失败 */
		ErrorCode m_errorInfo;			/*!< 注册失败时说明失败原因 */
		TaskEntityData taskInfo;
	};

	/*!
	 * @brief 创建任务侦测平台请求
	 */
	struct TaskDetectPlatformCreateRequest {
		TaskDetectPlatformCreateRequest() :m_detectId(0), m_platformPower(0), m_lastTime(0){
		}

		QString m_id;						/*!< id */
		QString m_taskId;					/*!< 任务标识 */
		int m_detectId;						/*!< 侦测平台标识 */
		QString m_name;						/*!< 侦测平台名称 */
		QString m_sensorType;				/*!< 侦测平台传感器类型 */
		int m_platformPower;				/*!< 平台动力 */
		QString m_startTime;				/*!< 开始任务时间 */
		QString m_endTime;					/*!< 结束任务时间 */
		int m_lastTime;						/*!< 时长 */
	};

	/*!
	 * @brief  创建任务侦测平台响应
	 */
	struct TaskDetectPlatformCreateResponse {
		TaskDetectPlatformCreateResponse() : m_createResult(false) { 
		}

		bool m_createResult;						/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;						/*!< 创建失败时说明失败原因 */
		TaskDetectPlatformEntityData m_dataInfo;
	};

	/*!
	 * @brief  查询所有任务侦测平台请求
	 */
	struct LoadAllTaskDetectPlatformRequest {
		LoadAllTaskDetectPlatformRequest() : m_offsetIndex(0), m_limitIndex(0) { 
		}

		QString m_taskId;					/*!< 任务Id */
		int m_offsetIndex;					/*!< 分页时，需加载的起始页偏移量 */
		int m_limitIndex;					/*!< 当前页面显示条数 */
	};

	/*!
	 * @brief  查询所有任务侦测平台响应
	 */
	struct LoadAllTaskDetectPlatformResponse {
		LoadAllTaskDetectPlatformResponse() : m_taskDetectPlatformCount(0) { 
		}

		int m_taskDetectPlatformCount;								/*!< 值班日志总条数 */
		QList<TaskDetectPlatformEntityData> m_dataInfos;		/*!< 当前页面下任务侦测平台结果集合 */
	};

	/*!
	 * @brief	按条件查询任务侦测平台请求
	 */
	struct TaskDetectPlatformByConditionRequest {

	};

	/*!
	 * @brief 按条件查询任务侦测平台响应
	 */
	struct TaskDetectPlatformByConditionResponse {
		TaskDetectPlatformByConditionResponse() : m_taskDetectPlatformCount(0) {
		}

		int m_taskDetectPlatformCount;								/*!< 值班日志总条数 */
		QList<TaskDetectPlatformEntityData> m_dataInfos;		/*!< 当前页面下任务侦测平台结果集合 */
	};

	/*!
	 * @brief  删除任务侦测平台请求
	 */
	struct TaskDetectPlatformDeleteRequest {
		TaskDetectPlatformDeleteRequest() {
		}

		QString m_id;						/*!< 唯一标识Id */
		QString m_taskId;					/*!< 任务Id */
	};

	/*!
	 * @brief  删除任务侦测平台响应
	 */
	struct TaskDetectPlatformDeleteResponse {
		TaskDetectPlatformDeleteResponse() : m_deleteResult(false) {
		}

		bool m_deleteResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief 修改任务侦测平台请求
	 */
	struct TaskDetectPlatformModifyRequest {
		TaskDetectPlatformModifyRequest() :m_detectId(0), m_platformPower(0), m_lastTime(0){
		}

		QString m_id;						/*!< 唯一标识 */
		QString m_taskId;					/*!< 任务标识 */
		int m_detectId;						/*!< 侦测平台标识 */
		QString m_name;						/*!< 侦测平台名称 */
		QString m_sensorType;				/*!< 侦测平台传感器类型 */
		int m_platformPower;				/*!< 平台动力 */
		QString m_startTime;				/*!< 开始任务时间 */
		QString m_endTime;					/*!< 结束任务时间 */
		int m_lastTime;						/*!< 时长 */
	};

	/*!
	 * @brief 修改任务侦测平台响应
	 */
	struct TaskDetectPlatformModifyResponse {
		TaskDetectPlatformModifyResponse() : m_modifyResult(false) {
		}

		bool m_modifyResult;					/*!< 注册结果，true:注册成功，false:注册失败 */
		ErrorCode m_errorInfo;					/*!< 注册失败时说明失败原因 */
		TaskDetectPlatformEntityData m_dataInfo;
	};

	/*!
	 * @brief  文件类型
	 */
	enum  FileType {
		File_Image,				/*!< 图片文件 */
		File_XML,				/*!< xml文件 */
		File_Dat,				/*!< dat文件 */
	};


	/*!
	 * @brief  任务数据文件创建请求
	 */
	struct TaskDataFileCreateRequest{
		TaskDataFileCreateRequest() :m_size(0){
		}

		QString m_taskId;					/*!< 任务标识 */
		QString m_detectId;					/*!< 侦测平台标识 */

		QString m_name;						/*!< 原始图片文件名 */
		QString m_suffix;					/*!< 图片类型 */
		QString m_timeStamp;				/*!< 时间标识 */
		qint64 m_size;						/*!< 图片大小 */
		QString m_md5;						/*!< Md5值 */
	};

	/*!
	 * @brief  任务数据文件创建相应
	 */
	struct TaskDataFileCreateResponse {
		TaskDataFileCreateResponse() :m_result(false) , m_lastLen(0){
		}

		bool m_result;						/*!< 创建结果，true:创建成功，false:创建失败 */
		QString m_taskId;					/*!< 任务标识 */
		QString m_md5;						/*!< Md5值 */
		QString m_id;						/*!< 文件标识Id */
		qint64 m_lastLen;					/*!< 上一次上传的长度 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief  任务数据文件删除请求
	 * @details
	 */
	struct TaskDataFileDeleteRequest {
		TaskDataFileDeleteRequest():m_fileType(0){
		}
		
		int m_fileType;						/*!< 文件类型 */
		QString m_id;						/*!< 唯一标识Id */
		QString m_taskId;					/*!< 任务Id */
	};

	/*!
	 * @brief  任务数据文件删除响应
	 * @details
	 */
	struct TaskDataFileDeleteResponse {
		TaskDataFileDeleteResponse() : m_deleteResult(false) {
		}

		bool m_deleteResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};


	/*!
	 * @brief  查询所有任务试验图片资源请求
	 * @details 
	 */
	struct LoadAllTaskImageRequest {
		LoadAllTaskImageRequest() :m_offsetIndex(0), m_limitIndex(0) { 
		}

		QString m_taskId;					/*!< 任务Id */
		int m_offsetIndex;					/*!< 分页时，需加载的起始页偏移量 */
		int m_limitIndex;					/*!< 当前页面显示条数 */
	};

	/*!
	 * @brief  查询所有任务试验图片资源响应
	 * @details 
	 */
	struct LoadAllTaskImageResponse {
		LoadAllTaskImageResponse() :m_taskImageCount(0) { 
		}

		int m_taskImageCount;								/*!< 值班日志总条数 */
		QList<TaskImageEntityData> m_taskImageInfos;		/*!< 当前页面下值班日志结果集合 */
	};

	/*!
	 * @brief  删除任务试验图片资源请求
	 * @details 
	 */
	struct TaskImageDeleteRequest {
		int fileType;
		QString m_id;						/*!< 唯一标识Id */
		QString m_taskId;					/*!< 任务Id */
	};

	/*!
	 * @brief  删除任务试验图片资源响应
	 * @details 
	 */
	struct TaskImageDeleteResponse {
		TaskImageDeleteResponse() : m_deleteResult(false) {
		}

		bool m_deleteResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief 值班日志创建请求报文
	 */
	struct DutyRecordCreateRequest {
		DutyRecordCreateRequest() :m_wind(0), m_windSpeed(0), m_waveHigh(0),m_oceanCurrents(0){
		}

		QString m_id;						/*!< id */
		QString m_taskId;					/*!< 任务标识 */
		QString m_createTime;				/*!< 创建时间 */
		QString m_description;				/*!< 描述信息 */
		QString m_seaCondition;				/*!< 海况 */
		double m_wind;						/*!< 风向 */
		double m_windSpeed;					/*!< 风速 */
		double m_waveHigh;					/*!< 浪高 */
		double m_oceanCurrents;				/*!< 洋流 */
	};

	/*!
	* @brief 值班日志创建请求结果报文
	*/
	struct DutyRecordCreateResponse	{
		DutyRecordCreateResponse(): m_createResult(false){
		}

		bool m_createResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
		DutyRecordEntityData m_dutyRecordInfo;
	};

	/*!
	 * @brief   加载所有值班日志请求
	 * @details 
	 */
	struct  LoadAllDutyRecordRequest {
		LoadAllDutyRecordRequest() :m_offsetIndex(0), m_limitIndex(0){
		}

		QString m_taskId;					/*!< 任务Id */
		int m_offsetIndex;					/*!< 分页时，需加载的起始页偏移量 */
		int m_limitIndex;					/*!< 当前页面显示条数 */
	};

	/*!
	 * @brief  加载所有值班日志请求结果报文
	 * @details 
	 */
	struct LoadAllDutyRecordResponse {
		LoadAllDutyRecordResponse() :m_dutyRecordCount(0){
		}

		int m_dutyRecordCount;								/*!< 值班日志总条数 */
		QList<DutyRecordEntityData> m_dutyRecordInfos;		/*!< 当前页面下值班日志结果集合 */
	};

	/*!
	 * @brief   值班日志删除请求
	 * @details 
	 */
	struct DutyRecordDeleteRequest{

		QString m_id;						/*!< Id */
		QString m_taskId;					/*!< 任务Id */
	};

	/*!
	 * @brief  值班日志删除请求结果报文
	 * @details 
	 */
	struct DutyRecordDeleteResponse {
		DutyRecordDeleteResponse() : m_deleteResult(false) {
		}

		bool m_deleteResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief   值班日志修改请求
	 * @details
	 */
	struct DutyRecordModifyRequest {
		DutyRecordModifyRequest() :m_wind(0), m_windSpeed(0), m_waveHigh(0),m_oceanCurrents(0) {
		}

		QString m_id;						/*!< id */
		QString m_taskId;					/*!< 任务标识 */
		QString m_createTime;				/*!< 创建时间 */
		QString m_description;				/*!< 描述信息 */
		QString m_seaCondition;				/*!< 海况 */
		double m_wind;						/*!< 风向 */
		double m_windSpeed;					/*!< 风速 */
		double m_waveHigh;					/*!< 浪高 */
		double m_oceanCurrents;				/*!< 洋流 */
	};

	/*!
	 * @brief  值班日志修改请求结果报文
	 * @details
	 */
	struct DutyRecordModifyResponse {
		DutyRecordModifyResponse() : m_modifyResult(false) {
		}

		bool m_modifyResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief 试验记录创建请求报文
	*/
	struct ExperimentRecordCreateRequest {
		ExperimentRecordCreateRequest() :m_lon(0), m_lat(0), m_setHeadingDegree(0),m_actualHeadingDegree(0),
			m_acousticState(0), m_targetNum(0), m_underwaterTargetNum(0),m_maxDepth(0), m_profileIndex(0), 
			m_profileLength(0), m_profileDistance(0) {
		}

		QString m_id;							/*!< 唯一标识 */
		QString m_taskId;						/*!< 任务标识 */
		QString m_platformId;					/*!< 平台标识 */
		QString m_floatingTime;					/*!< 上浮时间 */
		double m_lon;							/*!< 上浮经度 */
		double m_lat;							/*!< 上浮纬度 */
		double m_setHeadingDegree;				/*!< 设置航向角 */
		double m_actualHeadingDegree;			/*!< 实际航向角 */
		int m_acousticState;					/*!< 声学系统状态 */
		int m_targetNum;						/*!< 目标总数 */
		int m_underwaterTargetNum;				/*!< 水下目标数 */
		QString m_underwaterTargetInfo;			/*!< 水下目标信息 */
		double m_maxDepth;						/*!< 剖面最大深度 */
		int m_profileIndex;						/*!< 剖面序号 */
		double m_profileLength;					/*!< 剖面时长 */
		double m_profileDistance;				/*!< 剖面移动距离 */
	};

	/*!
	* @brief 试验记录创建请求结果报文
	*/
	struct ExperimentRecordCreateResponse {
		ExperimentRecordCreateResponse() : m_createResult(false) {
		}

		bool m_createResult;		/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;		/*!< 创建失败时说明失败原因 */
		ExperimentRecordEntityData m_experimentRecordInfo;
	};

	/*!
	 * @brief   加载所有试验记录请求
	* @details
	*/
	struct  LoadAllExperimentRecordsRequest {
		LoadAllExperimentRecordsRequest():m_offsetIndex(0), m_limitIndex(0){
		}

		QString m_taskId;					/*!< 任务Id */
		QString m_platformId;
		int m_offsetIndex;					/*!< 分页时，需加载的起始页偏移量 */
		int m_limitIndex;					/*!< 当前页面显示条数 */
	};

	/*!
	 * @brief  加载所有试验记录请求结果报文
	 * @details
	 */
	struct LoadAllExperimentRecordsResponse {
		LoadAllExperimentRecordsResponse() : m_experimentRecordCount(0){
		}

		int m_experimentRecordCount;													/*!< 总条数 */
		QList<ExperimentRecordEntityData > m_experimentRecordInfos;		/*!< 当前页面下试验记录结果集合 */
	};

	/*!
	* @brief   试验记录删除请求
	 * @details
	*/
	struct ExperimentRecordDeleteRequest {

		QString m_id;					/*!< Id */
		QString m_taskId;				/*!< 任务Id */
	};

	/*!
	 * @brief  试验记录删除请求结果报文
	 * @details
	 */
	struct ExperimentRecordDeleteResponse {
		ExperimentRecordDeleteResponse() : m_deleteResult(false) {
		}

		bool m_deleteResult;		/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;		/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief   试验记录修改请求
	 * @details
	 */
	struct ExperimentRecordModifyRequest {
		ExperimentRecordModifyRequest() :m_lon(0), m_lat(0), m_setHeadingDegree(0),m_actualHeadingDegree(0), 
			m_acousticState(0), m_targetNum(0), m_underwaterTargetNum(0),m_maxDepth(0), m_profileIndex(0), 
			m_profileLength(0), m_profileDistance(0) {
		}

		QString m_id;							/*!< 唯一标识 */
		QString m_taskId;						/*!< 任务标识 */
		QString m_platformId;					/*!< 平台标识 */
		QString m_floatingTime;					/*!< 上浮时间 */
		double m_lon;							/*!< 上浮经度 */
		double m_lat;							/*!< 上浮纬度 */
		double m_setHeadingDegree;				/*!< 设置航向角 */
		double m_actualHeadingDegree;			/*!< 实际航向角 */
		int m_acousticState;					/*!< 声学系统状态 */
		int m_targetNum;						/*!< 目标总数 */
		int m_underwaterTargetNum;				/*!< 水下目标数 */
		QString m_underwaterTargetInfo;			/*!< 水下目标信息 */
		double m_maxDepth;						/*!< 剖面最大深度 */
		int m_profileIndex;						/*!< 剖面序号 */
		double m_profileLength;					/*!< 剖面时长 */
		double m_profileDistance;				/*!< 剖面移动距离 */
	};

	/*!
	 * @brief  试验记录修改请求结果报文
	 * @details
	 */
	struct ExperimentRecordModifyResponse {
		ExperimentRecordModifyResponse() : m_modifyResult(false) {
		}

		bool m_modifyResult;		/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;		/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief 侦测平台创建请求报文
	 */
	struct DetectPlatformCreateRequest {
		DetectPlatformCreateRequest() :m_id(0) { 
		}

		int m_id;						/*!< id */
		QString m_name;					/*!< 平台名称 */
	};

	/*!
	* @brief 侦测平台创建请求结果报文
	*/
	struct  DetectPlatformCreateResponse {
		DetectPlatformCreateResponse() : m_createResult(false) { 
		}

		bool m_createResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
		DetectPlatformEntityData m_detectPlatformInfo;
	};

	/*!
	 * @brief   加载所有侦测平台请求
	 */
	struct  LoadAllDetectPlatformsRequest {
		LoadAllDetectPlatformsRequest():m_offsetIndex(0), m_limitIndex(0){
		}

		int m_offsetIndex;						/*!< 分页时，需加载的起始页偏移量 */
		int m_limitIndex;						/*!< 当前页面显示条数 */
	};

	/*!
	 * @brief  加载所有侦测平台请求结果报文
	 */
	struct LoadAllDetectPlatformsResponse {
		LoadAllDetectPlatformsResponse() : m_detectPlatformCount(0) { 
		}

		int m_detectPlatformCount;													/*!< 总条数 */
		QList<DetectPlatformEntityData > m_detectPlatformInfos;						/*!< 结果集合 */
	};

	/*!
	 * @brief  侦测平台删除请求
	 */
	struct DetectPlatformDeleteRequest {
		DetectPlatformDeleteRequest() :m_id(0){
		}

		int m_id;							/*!< Id */
	};

	/*!
	 * @brief  侦测平台删除请求结果报文
	 */
	struct DetectPlatformDeleteResponse {
		DetectPlatformDeleteResponse() : m_deleteResult(false) {
		}

		bool m_deleteResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief   侦测平台修改请求
	 */
	struct DetectPlatformModifyRequest {
		DetectPlatformModifyRequest() :m_id(0) {
		}

		int m_id;							/*!< 唯一标识 */
		QString m_name;						/*!< 平台名称 */
	};

	/*!
	 * @brief  侦测平台修改请求结果报文
	 */
	struct DetectPlatformModifyResponse {
		DetectPlatformModifyResponse() : m_modifyResult(false) {
		}

		bool m_modifyResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief 侦测平台亚型创建请求报文
	 */
	struct DetectPlatformSubtypeCreateRequest {
		DetectPlatformSubtypeCreateRequest() :m_id(0), m_detectId(0) { 
		}

		int m_id;						/*!< id */
		int m_detectId;					/*!< 侦测平台标识 */
		QString m_name;					/*!< 平台亚型名称 */
	};

	/*!
	* @brief 侦测平台亚型创建请求结果报文
	*/
	struct DetectPlatformSubtypeCreateResponse {
		DetectPlatformSubtypeCreateResponse() : m_createResult(false) {
		}

		bool m_createResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
		DetectPlatformSubtypeEntityData m_dutyRecordInfo;
	};

	/*!
	 * @brief   加载所有侦测平台亚型请求
	 */
	struct  LoadAllDetectPlatformSubtypesRequest {
		LoadAllDetectPlatformSubtypesRequest() : m_detectId(0) { 
		}

		int m_detectId;						/*!< 侦测平台标识 */
	};

	/*!
	 * @brief  加载所有侦测平台亚型请求结果报文
	 */
	struct LoadAllDetectPlatformSubtypesResponse {
		LoadAllDetectPlatformSubtypesResponse() : m_detectId(0), m_detectPlatformSubtypeCount(0) {
		}

		int m_detectId;
		int m_detectPlatformSubtypeCount;													/*!< 总条数 */
		QList<DetectPlatformSubtypeEntityData > m_detectPlatformSubtypeInfos;				/*!< 结果集合 */
	};

	/*!
	 * @brief  侦测平台亚型删除请求
	 */
	struct DetectPlatformSubtypeDeleteRequest {
		DetectPlatformSubtypeDeleteRequest() :m_id(0), m_detectId(0){
		}

		int m_id;							/*!< Id */
		int m_detectId;						/*!< 侦测平台标识 */
		QString m_name;
	};

	/*!
	 * @brief  侦测平台亚型删除请求结果报文
	 */
	struct DetectPlatformSubtypeDeleteResponse {
		DetectPlatformSubtypeDeleteResponse() : m_deleteResult(false) {
		}

		bool m_deleteResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief   侦测平台亚型修改请求
	 */
	struct DetectPlatformSubtypeModifyRequest {
		DetectPlatformSubtypeModifyRequest() : m_id(0), m_detectId(0){
		}

		int m_id;							/*!< 唯一标识 */
		int m_detectId;						/*!< 侦测平台标识 */
		QString m_name;						/*!< 平台名称 */
	};

	/*!
	 * @brief  侦测平台亚型修改请求结果报文
	 */
	struct DetectPlatformSubtypeModifyResponse {
		DetectPlatformSubtypeModifyResponse() : m_modifyResult(false) {
		}

		bool m_modifyResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief 目标数据信息创建请求
	 */
	struct  TargetCreateRequest	{
		TargetCreateRequest() :m_type(0), m_lon(0), m_lat(0), m_tonnage(0), m_speed(0), m_axlesNumber(0){
		}

		QString m_id;								/*!< 目标Id */
		QString m_name;								/*!< 目标名称 */
		int m_type;									/*!< 目标类型 */
		QString m_creanTime;						/*!< 创建时间 */
		double m_lon;								/*!< 经度 */
		double m_lat;								/*!< 纬度 */
		double m_tonnage;								/*!< 吨位 */
		double m_speed;								/*!< 航行速度 */
		int  m_axlesNumber;							/*!< 轴数 */
	};

	/*!
	 * @brief 目标创建请求结果报文
	 */
	struct  TargetCreateResponse {
		TargetCreateResponse() : m_createResult(false) {
		}

		bool m_createResult;						/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;						/*!< 创建失败时说明失败原因 */
		TargetEntityData m_targetInfo;		
	};

	/*!
	 * @brief   加载所有目标请求
	 */
	struct  LoadAllTargetRequest {
		LoadAllTargetRequest() :m_offsetIndex(0), m_limitIndex(0) {
		}

		int m_offsetIndex;							/*!< 分页时，需加载的起始页偏移量 */
		int m_limitIndex;							/*!< 当前页面显示条数 */
	};

	/*!
	 * @brief  加载所有目标请求结果报文
	 */
	struct LoadAllTargetResponse {
		LoadAllTargetResponse() :m_targetCount(0) {
		}

		int m_targetCount;								/*!< 值班日志总条数 */
		QList<TargetEntityData> m_targetInfos;			/*!< 当前页面下值班日志结果集合 */
	};

	/*!
	 * @brief  目标删除请求
	 */
	struct TargetDeleteRequest {
		TargetDeleteRequest() :m_id(0){
		}

		int m_id;											/*!< Id */
		QString m_name;
	};

	/*!
	 * @brief  目标删除请求结果报文
	 */
	struct TargetDeleteResponse {
		TargetDeleteResponse() : m_deleteResult(false) {
		}

		bool m_deleteResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief   目标修改请求
	 */
	struct TargetModifyRequest {
		QString m_id;							/*!< 唯一标识 */
		QString m_name;							/*!< 平台名称 */
		int m_type;
		QString m_createTime;
	};

	/*!
	 * @brief  目标修改请求结果报文
	 */
	struct TargetModifyResponse {
		TargetModifyResponse() : m_modifyResult(false) {}
		bool m_modifyResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief AIS数据信息创建请求
	 */
	struct  AISDataCreateRequest {
		AISDataCreateRequest() :m_mmsi(0), m_time(0), m_lon(0), m_lat(0), m_course(0), m_truehead(0),m_shipType(0),
			m_shipImo(0), m_navStatus(0), m_speed(0), m_length(0), m_width(0){
		}

		QString m_id;					/*!< 唯一标识 */
		QString m_targetId;				/*!< 目标标识 */
		int m_mmsi;						/*!< 船舶MMSI */
		int m_time;						/*!< 实时信号时间 */
		double m_lon;					/*!< 经度 */
		double m_lat;					/*!< 纬度 */
		double m_course;				/*!< 航迹向 */
		int  m_truehead;				/*!< 航首向 */
		QString m_name;					/*!< 船名 */
		int m_shipType;					/*!< 船舶以及货物类型 */
		int m_shipImo;					/*!< 船舶IMO */
		int m_navStatus;				/*!< 航行状态 */
		double m_speed;					/*!< 航行速度 */
		QString m_eta;					/*!< 预计到港时间 */
		QString m_dest;					/*!< 目的港 */
		double m_length;				/*!< 船长 */
		double m_width;					/*!< 船宽 */
		QString m_callsign;				/*!< 呼号 */
		QString m_flag;					/*!< 船旗 */
		QString m_buildDate;			/*!< 建造时间 */
		QString m_port;					/*!< 船籍港 */
	};

	/*!
	 * @brief AIS创建请求结果报文
	 */
	struct  AISDataCreateResponse {
		AISDataCreateResponse() : m_createResult(false) { 
		}

		bool m_createResult;						/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;						/*!< 创建失败时说明失败原因 */
		AisEntityData m_targetInfo;
	};
	
	/*!
	 * @brief  加载所有AIS数据请求
	 */
	struct  LoadAllAISDataRequest {
		LoadAllAISDataRequest() :m_offsetIndex(0), m_limitIndex(0) {
		}

		int m_offsetIndex;						/*!< 分页时，需加载的起始页偏移量 */
		int m_limitIndex;						/*!< 当前页面显示条数 */
	};

	/*!
	 * @brief  加载所有AIS数据请求响应
	 */
	struct LoadAllAISDatasResponse {
		LoadAllAISDatasResponse() : m_aisDataCount(0) { 
		}

		int m_aisDataCount;											/*!< 总条数 */
		QList<AisEntityData > m_aisDataInfos;						/*!< 结果集合 */
	};

	/*!
	 * @brief	按条件查询AIS数据请求
	 * @details
	 */
	struct AISDataByConditionRequest {
		AISDataByConditionRequest() {
		}

		QString m_id;					/*!< 唯一标识 */
		QString m_targetId;				/*!< 目标标识 */
	};

	/*!
	 * @brief  按条件查询AIS数据请求响应
	 * @details
	 */
	struct AISDataByConditionResponse {
		AISDataByConditionResponse() : m_aisDataCount(0) { 
		}

		int m_aisDataCount;											/*!< 总条数 */
		QList<AisEntityData > m_aisDataInfos;						/*!< 结果集合 */
	};

	/*!
	 * @brief  AIS数据删除请求
	 */
	struct AISDataDeleteRequest {
		AISDataDeleteRequest() {
		}

		QString m_id;					/*!< 唯一标识 */
		QString m_targetId;				/*!< 目标标识 */
	};

	/*!
	 * @brief  AIS数据删除请求结果报文
	 */
	struct AISDataDeleteResponse {
		AISDataDeleteResponse() : m_deleteResult(false) {
		}

		bool m_deleteResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief   AIS数据修改请求
	 */
	struct AISDataModifyRequest {
		AISDataModifyRequest() : m_mmsi(0), m_time(0), m_lon(0), m_lat(0),m_course(0), m_truehead(0) , m_shipType(0), 
			m_shipImo(0), m_navStatus(0), m_speed(0), m_length(0), m_width(0){
		}

		QString m_id;					/*!< 唯一标识 */
		QString m_targetId;				/*!< 目标标识 */
		int m_mmsi;						/*!< 船舶MMSI */
		int m_time;						/*!< 实时信号时间 */
		double m_lon;					/*!< 经度 */
		double m_lat;					/*!< 纬度 */
		double m_course;				/*!< 航迹向 */
		int  m_truehead;				/*!< 航首向 */
		QString m_name;					/*!< 船名 */
		int m_shipType;					/*!< 船舶以及货物类型 */
		int m_shipImo;					/*!< 船舶IMO */
		int m_navStatus;				/*!< 航行状态 */
		double m_speed;					/*!< 航行速度 */
		QString m_eta;					/*!< 预计到港时间 */
		QString m_dest;					/*!< 目的港 */
		double m_length;				/*!< 船长 */
		double m_width;					/*!< 船宽 */
		QString m_callsign;				/*!< 呼号 */
		QString m_flag;					/*!< 船旗 */
		QString m_buildDate;			/*!< 建造时间 */
		QString m_port;					/*!< 船籍港 */
	};

	/*!
	 * @brief  AIS数据修改请求响应
	 */
	struct AISDataModifyResponse {
		AISDataModifyResponse() : m_modifyResult(false) {
		}

		bool m_modifyResult;				/*!< 创建结果，true:创建成功，false:创建失败 */
		ErrorCode m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};



} // namespace Datastruct

#endif // DATASTRUCT_H
