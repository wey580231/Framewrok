#include "filesession.h"

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

	}

} //namespace Related 