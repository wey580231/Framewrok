
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

#include <base/util/rringbuffer.h>
#include <commondefines/protocol.h>
#include <network/libuv/uv_tcpserver.h>
#include "../related_global.h"

namespace Related {

	/*!
	 * @brief INI配置文件key
	 */
	struct ConfigKey {
		QString m_netGroupId = "LocalServer";

		QString m_localDataServerIp = "DataServerIp";
		QString m_localDataPort = "DataServerPort";
		QString m_localFilePort = "FileServerPort";

		QString m_dbGroupId = "Database";
		QString m_dbType = "DatabaseType";
		QString m_dbHost = "HostName";
		QString m_dbName = "DatabaseName";
		QString m_dbPort = "Port";
		QString m_userName = "UserName";
		QString m_password = "Password";
		QString m_userInnerPassword = "UsePasswordInProgram";
	};

	/*!
	 * @brief 单个客户端网络信息
	 * @details 保存客户端网络连接、数据接收缓冲区，每个客户端连接拥有一个此对象
	 */
	struct RemoteClientInfo {
		RemoteClientInfo() :m_tcpClient(nullptr), m_recvRingBuffer(M_BYTES(2)) {}
		Network::AcceptTcpClient * m_tcpClient;		/*!< 客户端连接 */
		Base::RFixedRingBuffer m_recvRingBuffer;	/*!< 客户端对应的接收环形缓冲区 */
	};

	/*!
	 * @brief 单次网络数据请求
	 * @details 服务器接收到请求后，封装成请求体，交由处理线程解析、根据类型处理。
	 */
	struct RequestUnit {
		RequestUnit() :m_clientId(0) {}
		int m_clientId;				/*!< 客户端tcp连接标识，可通过此标识找到网络连接 */
		QByteArray m_requestData;	/*!< 数据请求体，包括数据包头、包尾，由处理线程自行解析 */
	};

	/*!
	 * @brief 单次处理结果响应
	 * @details 服务器处理完请求后，需对结果进行打包，根据连接标识，发送回客户端。
	 */
	struct ResponseUnit {
		int m_clientId;				/*!< 客户端tcp连接标识，可通过此标识找到网络连接 */
		QByteArray m_resposneData;	/*!< 响应数据体 */
	};

} //namespace Related


#endif // DATASTRUCT_H
