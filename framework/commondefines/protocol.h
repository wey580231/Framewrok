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
		P_UserLogin = 1,		/*!< 用户登录 */
		P_UserRegist,			/*!< 用户注册 */
		P_UserList,				/*!< 用户列表查询 */
		P_UserOperate,			/*!< 用户信息更新,包括权限更新、删除用户 */

		P_CreateTask = 10,		/*!< 创建任务 */
		P_TaskList,				/*!< 查询所有任务 */
		P_TaskByCondition,		/*!< 按条件查询任务 */
		P_TaskDelete,			/*!< 删除任务 */
		P_TaskStaticsInfo,		/*!< 任务统计信息，包括任务数、占用空间大小等 */
		P_TaskSimpleInfo,		/*!< 单个任务概览信息 */
		P_TaskFullInfo,			/*!< 单个任务详细信息 */

		P_CreateDutyRecord = 20,	/*!< 创建值班日志 */
		P_ListDutyRecords ,			/*!< 查询所有值班日志 */
		P_DutyRecordByCondition,	/*!< 按条件查询值班日志 */
		P_DeleteDutyRecords,		/*!< 删除值班日志-单条 */
		P_ModifyDutyRecord,			/*!< 修改值班日志 */

		P_CreateExperimentRecord = 30,	/*!< 创建试验记录 */
		P_ListExperimentRecords,		/*!< 查询所有试验记录 */
		P_ExperimentRecordByCondition,	/*!< 按条件查询试验记录 */
		P_DeleteExperimentRecord,		/*!< 删除试验记录 - 单条*/
		P_ModifyExperimentRecord,		/*!< 修改试验记录 */

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

	struct PacketTail
	{
		PacketTail() :m_magicTail(PACK_TAIL) {}
		uint m_magicTail;		/*!< 报文尾标识 */
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
		UserLoginResponse() :m_loginResult(false) {}
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
	 * @brief 任务创建响应
	 * @details 
	 */
	struct TaskCreateResponse {
		TaskCreateResponse() : m_createResult(false) {
		}
		bool m_createResult;		/*!< 注册结果，true:注册成功，false:注册失败 */
		QString m_errorInfo;		/*!< 注册失败时说明失败原因 */

		TaskEntityData taskInfo;
	};

	/*!
	* @brief   加载所任务请求
	* @details
	*/
	struct  LoadAllTaskRequest {
		LoadAllTaskRequest():m_offsetIndex(0), m_limitIndex(0){

		}
		QString taskId;				/*!< 任务Id */
		int m_offsetIndex;			/*!< 分页时，需加载的起始页偏移量 */
		int m_limitIndex;			/*!< 当前页面显示条数 */
	};

	/*!
	* @brief  加载所有任务响应
	* @details
	*/
	struct LoadAllTaskResponse {
		int m_count;									/*!< 任务总条数 */
		QList<TaskEntityData> m_taskInfos;				/*!< 当前页面下任务结果集合 */
	};

	/*!
	 * @brief   按条件查询任务请求
	 * @details 
	 */
	struct TaskByConditionRequest {
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
	 * @brief   按条件查询任务响应
	 * @details 
	 */
	struct TaskByConditionResponse {
		int m_count;									/*!< 任务总条数 */
		QList<TaskEntityData> m_taskInfos;				/*!< 当前页面下任务结果集合 */
	};

	/*!
	 * @brief   任务删除请求
	 * @details
	 */
	struct TaskDeleteRequest {
		QString taskId;				/*!< 任务Id */
	};

	/*!
	 * @brief  任务删除请求结果报文
	 * @details
	 */
	struct TaskDeleteResponse {
		TaskDeleteResponse() : m_deleteResult(false) {
		}
		bool m_deleteResult;					/*!< 创建结果，true:创建成功，false:创建失败 */
		QString m_errorInfo;					/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief  任务统计信息请求
	 * @details 
	 */
	struct  TaskStaticsInfoRequest{

	};

	/*!
	 * @brief  任务统计信息响应
	 * @details
	 */
	struct  TaskStaticsInfoResponse {

	};

	/*! 
	 * @brief  单条任务预览信息请求
	 * @details 
	 */
	struct TaskSimpleRequest{
		QString taskId;				/*!< 任务Id */
	};

	/*!
	 * @brief  单条任务预览信息响应
	 * @details
	 */
	struct TaskSimpleResponse {
		TaskSimpleResponse() : m_result(false) {
		}
		bool m_result;					/*!< 注册结果，true:注册成功，false:注册失败 */
		QString m_errorInfo;			/*!< 注册失败时说明失败原因 */

		TaskEntityData taskInfo;
	};

	/*!
	 * @brief    单个任务详细信息
	 * @details 
	 */
	struct TaskFullInfoRequest {

	};

	/*!
	 * @brief    单个任务详细信息
     * @details
     */
	struct TaskFullInfoResponse {

	};

	/*!
	 * @brief 值班日志创建请求报文
	 */
	struct DutyRecordCreateRequest {
		DutyRecordCreateRequest() :m_wind(0), m_windSpeed(0), m_waveHigh(0),
			m_oceanCurrents(0){
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
		QString m_errorInfo;				/*!< 创建失败时说明失败原因 */

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
		QString m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief   值班日志修改请求
	 * @details
	 */
	struct DutyRecordModifyRequest {
		DutyRecordModifyRequest() :m_wind(0), m_windSpeed(0), m_waveHigh(0),
			m_oceanCurrents(0) {
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
		QString m_errorInfo;				/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief 试验记录创建请求报文
	*/
	struct ExperimentRecordCreateRequest {
		ExperimentRecordCreateRequest() :m_lon(0), m_lat(0), m_setHeadingDegree(0),
			m_actualHeadingDegree(0), m_acousticState(0), m_targetNum(0), m_underwaterTargetNum(0),
			m_maxDepth(0), m_profileIndex(0), m_profileLength(0), m_profileDistance(0) {
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
		QString m_errorInfo;		/*!< 创建失败时说明失败原因 */

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
		QString m_errorInfo;		/*!< 创建失败时说明失败原因 */
	};

	/*!
* @brief   试验记录修改请求
 * @details
*/
	struct ExperimentRecordModifyRequest {
		ExperimentRecordModifyRequest() :m_lon(0), m_lat(0), m_setHeadingDegree(0),
			m_actualHeadingDegree(0), m_acousticState(0), m_targetNum(0), m_underwaterTargetNum(0),
			m_maxDepth(0), m_profileIndex(0), m_profileLength(0), m_profileDistance(0) {
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
		QString m_errorInfo;		/*!< 创建失败时说明失败原因 */
	};

} // namespace Datastruct

#endif // DATASTRUCT_H
