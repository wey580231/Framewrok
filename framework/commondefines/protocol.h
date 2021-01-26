﻿/*!
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
		P_UserUpdate,			/*!< 用户信息更新,包括权限更新 */
		P_UserDelete,			/*!< 删除用户 */

		P_CreateTask = 10,		/*!< 创建任务 */
		P_TaskList,				/*!< 查询所有任务 */
		P_TaskByCondition,		/*!< 按条件查询任务 */
		P_TaskDelete,			/*!< 删除任务 */
		P_TaskStaticsInfo,		/*!< 任务统计信息，包括任务数、占用空间大小等 */
		P_TaskSimpleInfo,		/*!< 单个任务概览信息 */
		P_TaskFullInfo,			/*!< 单个任务详细信息 */

		P_CreateDutyRecord = 20,/*!< 创建值班日志 */
		P_ListDutyRecords ,		/*!< 查询所有值班日志 */
		P_DeleteDutyRecords,	/*!< 删除值班日志 */
		P_ModifyDutyRecord,		/*!< 修改值班日志 */

		P_CreateExperimentRecord = 30,	/*!< 创建试验记录 */
		P_ListExperimentRecords,		/*!< 查询所有试验记录 */
		P_DeleteExperimentRecord,		/*!< 删除试验记录 */
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
		QString m_errorInfo;	/*!< 登录失败时说明失败原因 */

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
		QString m_errorInfo;	/*!< 注册失败时说明失败原因 */
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
		LoadAllUserResponse():m_userCount(0){}
		int m_userCount;		/*!< 用户总条数 */
		QList<UserEntityData> m_userInfos;		/*!< 当前页面下用户结果集合 */
	};
	
	/*!
	 * @brief 创建任务请求
	 */
	struct  TaskCreateRequest{
		QString taskId;
		QString taskName;
		
	};

	/*!
	 * @brief 任务创建结果
	 * @details 
	 */
	struct  TaskCreateResponse
	{
		bool m_createResult;		/*!< 注册结果，true:注册成功，false:注册失败 */
		QString m_errorInfo;		/*!< 注册失败时说明失败原因 */
	};

	/*!
	 * @brief 值班日志创建请求报文
	 */
	struct DutyRecordCreateRequest
	{
		QString id;					/*!< id */
		QString taskId;				/*!< 任务Id  */
		QString createTime;			/*!< 创建时间 */
	};

	/*!
	* @brief 值班日志创建请求结果报文
	*/
	struct DutyRecordCreateResponse
	{
		DutyRecordCreateResponse(): m_createResult(false){
		}
		bool m_createResult;		/*!< 创建结果，true:创建成功，false:创建失败 */
		QString m_errorInfo;		/*!< 创建失败时说明失败原因 */

		DutyRecordEntityData dutyRecordInfo;
	};

	/*!
	 * @brief   加载所有值班日志请求
	 * @details 
	 */
	struct  LoadAllDutyRecordRequest
	{
		QString taskId;				/*!< 任务Id */
		int m_offsetIndex;			/*!< 分页时，需加载的起始页偏移量 */
		int m_limitIndex;			/*!< 当前页面显示条数 */
	};

	/*!
	 * @brief  加载所有值班日志请求结果报文
	 * @details 
	 */
	struct LoadAllDutyRecordResponse {
		int m_dutyRecordCount;								/*!< 值班日志总条数 */
		QList<DutyRecordEntityData> m_dutyRecordInfos;		/*!< 当前页面下值班日志结果集合 */
	};

	/*!
	 * @brief   值班日志删除请求
	 * @details 
	 */
	struct DutyRecordDeleteRequest
	{
		QString id;					/*!< Id */
		QString taskId;				/*!< 任务Id */
	};

	/*!
	 * @brief  值班日志删除请求结果报文
	 * @details 
	 */
	struct DutyRecordDeleteResponse
	{
		DutyRecordDeleteResponse() : m_deleteResult(false) {
		}
		bool m_deleteResult;		/*!< 创建结果，true:创建成功，false:创建失败 */
		QString m_errorInfo;		/*!< 创建失败时说明失败原因 */
	};

	/*!
	 * @brief 试验记录创建请求报文
	*/
	struct ExperimentRecordCreateRequest
	{
		QString id;				/*!< 数据库ID */
		QString taskId;			/*!< 任务Id */
		QString platformId;		/*!< 平台Id */
		double lon;				/*!< 经度 */
		double lat;				/*!< 纬度 */
		QString seaCondition;	/*!< 海况 */
		QString floatingTime;	/*!< 浮动时间 */
		QString createTime;		/*!< 记录时间 */
	};

	/*!
	* @brief 试验记录创建请求结果报文
	*/
	struct ExperimentRecordCreateResponse
	{
		ExperimentRecordCreateResponse() : m_createResult(false) {
		}
		bool m_createResult;		/*!< 创建结果，true:创建成功，false:创建失败 */
		QString m_errorInfo;		/*!< 创建失败时说明失败原因 */

		ExperimentRecordEntityData dutyRecordInfo;
	};

	/*!
	 * @brief   加载所有试验记录请求
	* @details
	*/
	struct  LoadAllExperimentRecordsRequest
	{
		QString taskId;				/*!< 任务Id */
		int m_offsetIndex;			/*!< 分页时，需加载的起始页偏移量 */
		int m_limitIndex;			/*!< 当前页面显示条数 */
	};

	/*!
	 * @brief  加载所有试验记录请求结果报文
	 * @details
	 */
	struct LoadAllExperimentRecordsResponse {
		int m_count;										/*!< 总条数 */
		QList<ExperimentRecordEntityData> m_listInfos;		/*!< 当前页面下试验记录结果集合 */
	};

	/*!
	* @brief   值班日志删除请求
	 * @details
	*/
	struct ExperimentRecordDeleteRequest
	{
		QString id;					/*!< Id */
		QString taskId;				/*!< 任务Id */
	};

	/*!
	 * @brief  值班日志删除请求结果报文
	 * @details
	 */
	struct ExperimentRecordDeleteResponse
	{
		ExperimentRecordDeleteResponse() : m_deleteResult(false) {
		}
		bool m_deleteResult;		/*!< 创建结果，true:创建成功，false:创建失败 */
		QString m_errorInfo;		/*!< 创建失败时说明失败原因 */
	};

} // namespace Datastruct

#endif // DATASTRUCT_H
