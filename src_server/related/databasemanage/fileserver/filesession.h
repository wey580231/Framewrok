/*!
 * @brief     文件客户端连接句柄   
 * @author    wey
 * @version   1.0
 * @date      2021.01.30 14:58:42
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

#include "../net/tcpsession.h"

namespace Related {

	class FileSession : public TcpSession
	{

	public:
		FileSession(Network::AcceptTcpClient * acceptedClient);
		~FileSession();

	protected:
		void processData(QByteArray & requestData);

	};

} //namespace Related 