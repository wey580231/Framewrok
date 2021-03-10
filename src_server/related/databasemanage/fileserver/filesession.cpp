#include "filesession.h"

#include <QDebug>


#include "../global.h"

namespace Related {

	FileSession::FileSession(Network::AcceptTcpClient * acceptedClient)
		: TcpSession(acceptedClient)
	{
	}

	FileSession::~FileSession()
	{
	}

	void FileSession::processData(QByteArray & requestData)
	{
		qDebug() << "____________________FileSession____________________________________";

		FileRequestUnit * unit = new FileRequestUnit();
		unit->m_clientId = m_tcpClient->id();
		unit->m_requestData.swap(requestData);

		G_FileRequestQuque.put(unit);
	}

} //namespace Related 