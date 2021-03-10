/*!
 * @brief     文件传输链接   
 * @author    wey
 * @version   1.0
 * @date      2021.02.02 08:18:12
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

#include "netconnector.h"

namespace Related {

	class FileNetConnector : public NetConnector
	{
		Q_OBJECT

	public:
		static FileNetConnector * instance();
		~FileNetConnector();

		void write(QByteArray  buff);

	protected:
		void processNetData(QByteArray & data);

		
	private:
		FileNetConnector();
		QByteArray makePacket(Datastruct::PacketType type, QByteArray & body);

	private:
		static FileNetConnector * m_instance;
	};

} //namespace Related 