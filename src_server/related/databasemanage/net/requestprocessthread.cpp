#include "requestprocessthread.h"

#include <QDebug>

#include "../global.h"
#include "../datastruct.h"
#include "protocol.h"
#include "jsonwrapper.h"

namespace Related {

	RequestProcessThread::RequestProcessThread(QObject *parent)
		: Core::RTask(parent)
	{
	}

	RequestProcessThread::~RequestProcessThread()
	{
	}

	void RequestProcessThread::startMe()
	{
		RTask::startMe();
		runningFlag = true;
		start();
	}

	void RequestProcessThread::stopMe()
	{
		RTask::stopMe();
		runningFlag = false;
		G_RequestQuque.wakeUpConsumer(true);
	}

	void RequestProcessThread::run()
	{
		while (runningFlag) {

			while (runningFlag && G_RequestQuque.size() == 0) {
				G_RequestQuque.wait();
			}

			if (runningFlag) {
				//[1]
				std::list<RequestUnit *> dataList = G_RequestQuque.takeAll();

				std::for_each(dataList.begin(), dataList.end(), [&](RequestUnit * unit) {
					parseRequest(unit);
				});
			}
		}
	}

	/*!
	 * @brief 解析网络请求体
	 * @param unit 网络请求单元
	 */
	void RequestProcessThread::parseRequest(RequestUnit * unit)
	{
		PacketHead head;
		memcpy((char *)&head, unit->m_requestData.data(), sizeof(PacketHead));

		QByteArray jsonData(unit->m_requestData.data() + sizeof(head), unit->m_requestData.size() - sizeof(PacketHead) - sizeof(PacketTail));

		switch (head.m_packetType)
		{
			case P_UserLogin: {
				UserLoginRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {

					ResponseUnit * runit = new ResponseUnit();
					UserLoginResponse response;
					response.m_loginResult = true;

					runit->m_clientId = unit->m_clientId;
					runit->m_resposneData = makePacket(P_UserLogin,JsonWrapper::instance()->wrap(P_UserLogin,response));

					//发送回主线程
					sendProcessResult(runit);
				}
			}
				break;
			default:
				break;
		}

		delete unit;
	}

	QByteArray RequestProcessThread::makePacket(PacketType type, const QByteArray & body)
	{
		static int headLen = sizeof(PacketHead);
		static int tailLen = sizeof(PacketTail);

		PacketHead phead;
		phead.m_packetType = type;
		phead.m_dataLen = headLen + body.length() + tailLen;

		PacketTail ptail;

		QByteArray data;
		data.append((char *)&phead, headLen);
		data.append(body);
		data.append((char *)&ptail, tailLen);

		return data;
	}

} //namespace Related 