/*!
 * @brief     客户端和服务器端通信数据协议
 * @attention 1.每条协议说明清楚由谁发送、谁接收；
			  2.若由客户端发送至服务器，则统一以Request结尾；
			  3.若由服务器发回客户端，则统一以Response结尾；
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
		P_TaskFullInfo			/*!< 单个任务详细信息 */
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
	 * @details 客户端->服务器端
	 */
	struct UserLoginRequest {
		QString m_name;			/*!< 用户名 */
		QString m_password;		/*!< 密码，转换成MD5 */
	};

	/*!
	 * @brief 用户登录结果报文
	 * @details 服务器端->客户端
	 */
	struct UserLoginResponse {
		UserLoginResponse():m_loginResult(false){}
		bool m_loginResult;		/*!< 登录结果信息，true:登录成功，false:登录失败 */
		QString m_errorInfo;	/*!< 登录失败时说明失败原因 */

		UserEntityData m_userInfo;	/*!< 登录成功后，返回基本信息；失败不返回 */
	};


} // namespace Datastruct

#endif // DATASTRUCT_H
