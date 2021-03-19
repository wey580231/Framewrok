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
#include <QFile>

#include "netconnector.h"

namespace Related {

	class FileNetConnector : public NetConnector
	{
		Q_OBJECT

	public:
		static FileNetConnector * instance();
		~FileNetConnector();

		void write(Datastruct::FileInfoParameter  parameter, QByteArray data);
	
	signals:
		void signalSendFileStatus();

	protected:
		void processNetData( QByteArray & data);

		
	private:
		FileNetConnector();
		QByteArray makePacket(Datastruct::PacketType type, Datastruct::FileInfoParameter parameter, QByteArray data);

	private:
		static FileNetConnector * m_instance;
	};

} //namespace Related 