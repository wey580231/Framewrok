#include "jsonwrapper.h"

#include "signaldispatch.h"

namespace Related {

	JsonWrapper * JsonWrapper::m_instance = nullptr;

	JsonWrapper::JsonWrapper(QObject *parent)
		: QObject(parent)
	{
	}

	JsonWrapper * JsonWrapper::instance()
	{
		if (m_instance == nullptr)
			m_instance = new JsonWrapper();

		return m_instance;
	}

	JsonWrapper::~JsonWrapper()
	{
	}

	QByteArray JsonWrapper::wrap(PacketType type, const UserLoginRequest & request)
	{
		QJsonObject obj;
		obj.insert("name", request.m_name);
		obj.insert("password", request.m_password);

		QJsonDocument doc;
		doc.setObject(obj);

		return doc.toJson(QJsonDocument::Compact);
	}

	void JsonWrapper::unwrap(const QByteArray & array)
	{
		PacketHead head;
		memcpy((char *)&head, array.data(), sizeof(PacketHead));

		QByteArray jsonData(array.data() + sizeof(head), array.size() - sizeof(PacketHead) - sizeof(PacketTail));

		QJsonObject jsonObject = QJsonDocument::fromJson(jsonData).object();

		if (!jsonObject.isEmpty())
			return;

		switch (head.m_packetType)
		{
			case P_UserLogin: {
				UserLoginResponse response;
				response.m_loginResult = jsonObject.value("name").toBool();
				response.m_errorInfo = jsonObject.value("password").toString();
				
				SignalDispatch::instance()->recvUserLoginResponse(response);
			}
				break;
			default:
				break;
		}
	}

} //namespace Related 