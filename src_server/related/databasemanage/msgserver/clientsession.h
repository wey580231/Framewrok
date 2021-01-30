/*!
 * @brief     普通数据请求客户端   
 * @author    wey
 * @version   1.0
 * @date      2021.01.30 14:14:21
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

#include "../net/tcpsession.h"

namespace Related {

	class ClientSession : public TcpSession
	{
	public:
		ClientSession(Network::AcceptTcpClient * acceptedClient);
		~ClientSession();

	protected:
		void processData(QByteArray & requestData);
	};

} //namespace Related 