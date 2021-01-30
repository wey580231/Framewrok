#include "clientsession.h"

#include "../global.h"

namespace Related {

	ClientSession::ClientSession(Network::AcceptTcpClient * client):
		TcpSession(client)
	{
	}

	ClientSession::~ClientSession()
	{
	}

	void ClientSession::processData(QByteArray & requestData)
	{
		RequestUnit * unit = new RequestUnit();
		unit->m_clientId = m_tcpClient->id();
		unit->m_requestData.swap(requestData);

		G_RequestQuque.put(unit);
	}

} //namespace Related 