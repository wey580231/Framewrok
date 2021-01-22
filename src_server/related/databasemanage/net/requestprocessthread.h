#pragma once

#include <QObject>

#include <core/network/rtask.h>

#include "../net/protocol.h"
#include "../datastruct.h"

namespace Related {

	struct RequestUnit;

	class RequestProcessThread : public Core::RTask
	{
		Q_OBJECT

	public:
		RequestProcessThread(QObject *parent = nullptr);
		~RequestProcessThread();

		void startMe();
		void stopMe();

	signals:
		void sendProcessResult(ResponseUnit * result);

	protected:
		void run();

	private:
		void parseRequest(RequestUnit * unit);
		QByteArray makePacket(PacketType type, const QByteArray & body);
	};

} //namespace Related 