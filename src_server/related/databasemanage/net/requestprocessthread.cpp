#include "requestprocessthread.h"

#include <QDebug>

#include <commondefines/protocol.h>
#include <commondefines/wrapper/jsonwrapper.h>
#include <base\util\rlog.h>

#include "../global.h"
#include "../datastruct.h"

using namespace CommonDefines;

namespace Related {

	RequestProcessThread::RequestProcessThread(QObject *parent)
		: Core::RTask(parent), m_dbConnect(nullptr)
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
		m_dbConnect = Base::DatabaseManager::instance()->newDatabase();
		if (m_dbConnect == nullptr) {
			RLOG_ERROR("create database error!");
			return;
		}

		m_processCenter.bindDatabase(m_dbConnect);

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
		Datastruct::PacketHead head;
		memcpy((char *)&head, unit->m_requestData.data(), sizeof(Datastruct::PacketHead));

		QByteArray jsonData(unit->m_requestData.data() + sizeof(head), unit->m_requestData.size() - sizeof(Datastruct::PacketHead) - sizeof(Datastruct::PacketTail));

		ResponseUnit * runit = new ResponseUnit();
		runit->m_clientId = unit->m_clientId;

		switch (head.m_packetType)
		{
			case Datastruct::P_UserLogin: {
				Datastruct::UserLoginRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::UserLoginResponse response = m_processCenter.processUserLogin(unit->m_clientId,request);
					runit->m_resposneData = makePacket(Datastruct::P_UserLogin,JsonWrapper::instance()->wrap(response));
				}
			}
				break;

			case Datastruct::P_UserRegist: {
				Datastruct::UserRegistRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::UserRegistResponse response = m_processCenter.processUserRegist(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_UserRegist, JsonWrapper::instance()->wrap(response));
				}
			}
				break;

			case Datastruct::P_UserList: {
				Datastruct::LoadAllUserRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::LoadAllUserResponse response = m_processCenter.processUserList(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_UserList, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			default:
				break;
		}

		//发送回主线程
		if (runit->m_resposneData.size() > 0) {
			sendProcessResult(runit);
		}

		delete unit;
	}

	QByteArray RequestProcessThread::makePacket(Datastruct::PacketType type, const QByteArray & body)
	{
		static int headLen = sizeof(Datastruct::PacketHead);
		static int tailLen = sizeof(Datastruct::PacketTail);

		Datastruct::PacketHead phead;
		phead.m_packetType = type;
		phead.m_dataLen = headLen + body.length() + tailLen;

		Datastruct::PacketTail ptail;

		QByteArray data;
		data.append((char *)&phead, headLen);
		data.append(body);
		data.append((char *)&ptail, tailLen);

		return data;
	}

} //namespace Related 