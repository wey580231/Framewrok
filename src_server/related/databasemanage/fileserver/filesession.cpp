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

		FileRequestUnit * unit = new FileRequestUnit();
		unit->m_clientId = m_tcpClient->id();
		unit->m_requestData.swap(requestData);

		//G_FileRequestQuque.put(unit);

		g_mutex_FileRequestQueueMutex.lock();
		g_FileRequestQueue.push_back(unit);
		g_mutex_FileRequestQueueMutex.unlock();
		g_cv_FileRequestQuqueCondition.notify_one();
	}

} //namespace Related 