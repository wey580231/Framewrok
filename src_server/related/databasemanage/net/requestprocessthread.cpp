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
			case Datastruct::P_UserOperate: {
				Datastruct::OperateUserRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::OperateUserResponse response = m_processCenter.processUserOperate(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_UserOperate, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_CreateTask: {
				Datastruct::TaskCreateRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TaskCreateResponse response = m_processCenter.processTaskCreate(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_CreateTask, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_TaskList: {
				Datastruct::LoadAllTaskRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::LoadAllTaskResponse response = m_processCenter.processTaskList(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TaskList, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_TaskByCondition: {

			}
				break;
			case  Datastruct::P_TaskDelete: {
				Datastruct::TaskDeleteRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TaskDeleteResponse response = m_processCenter.processTaskDelete(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TaskDelete, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_TaskStaticsInfo: {

			}
				break;
			case  Datastruct::P_TaskSimpleInfo: {
				Datastruct::TaskSimpleRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TaskSimpleResponse response = m_processCenter.processTaskSimple(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TaskSimpleInfo, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_TaskFullInfo: {

			}
				break;
			case  Datastruct::P_CreateDutyRecord: {
				Datastruct::DutyRecordCreateRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::DutyRecordCreateResponse response = m_processCenter.processDutyRecordCreate(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_CreateDutyRecord, JsonWrapper::instance()->wrap(response));
				}
			}
				break;  
			case Datastruct::P_ListDutyRecords:{
				Datastruct::LoadAllDutyRecordRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::LoadAllDutyRecordResponse response = m_processCenter.processDutyRecordList(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_ListDutyRecords, JsonWrapper::instance()->wrap(response));
				}
			 }
				break;
			case Datastruct::P_DeleteDutyRecords:{
				Datastruct::DutyRecordDeleteRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::DutyRecordDeleteResponse response = m_processCenter.processDutyRecordDelete(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_DeleteDutyRecords, JsonWrapper::instance()->wrap(response));
				}
			 }
				break;
			case Datastruct::P_ModifyDutyRecord: {
				Datastruct::DutyRecordModifyRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::DutyRecordModifyResponse response = m_processCenter.processDutyRecordModify(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_ModifyDutyRecord, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case Datastruct::P_CreateExperimentRecord: {
				Datastruct::ExperimentRecordCreateRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::ExperimentRecordCreateResponse response = m_processCenter.processExperimentRecordCreate(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_CreateExperimentRecord, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case Datastruct::P_ListExperimentRecords: {
				Datastruct::LoadAllExperimentRecordsRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::LoadAllExperimentRecordsResponse response = m_processCenter.processExperimentRecordList(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_ListExperimentRecords, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case Datastruct::P_DeleteExperimentRecord: {
				Datastruct::ExperimentRecordDeleteRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::ExperimentRecordDeleteResponse response = m_processCenter.processExperimentRecordDelete(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_DeleteExperimentRecord, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case Datastruct::P_ModifyExperimentRecord: {
				Datastruct::ExperimentRecordModifyRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::ExperimentRecordModifyResponse response = m_processCenter.processExperimentRecordModify(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_ModifyExperimentRecord, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case Datastruct::P_DetectPlatformCreate: {
				Datastruct::DetectPlatformCreateRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::DetectPlatformCreateResponse response = m_processCenter.processDetectPlatformCreate(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_DetectPlatformCreate, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case Datastruct::P_DetectPlatformList: {
				Datastruct::LoadAllDetectPlatformsRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::LoadAllDetectPlatformsResponse response = m_processCenter.processDetectPlatformList(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_DetectPlatformList, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case Datastruct::P_DetectPlatformDelete: {
				Datastruct::DetectPlatformDeleteRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::DetectPlatformDeleteResponse response = m_processCenter.processDetectPlatformDelete(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_DetectPlatformDelete, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case Datastruct::P_DetectPlatformModify: {
				Datastruct::DetectPlatformModifyRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::DetectPlatformModifyResponse response = m_processCenter.processDetectPlatformModify(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_DetectPlatformModify, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case Datastruct::P_DetectPlatformSubtypeCreate: {
				Datastruct::DetectPlatformSubtypeCreateRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::DetectPlatformSubtypeCreateResponse response = m_processCenter.processDetectPlatformSubtypeCreate(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_DetectPlatformSubtypeCreate, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case Datastruct::P_DetectPlatformSubtypeList: {
				Datastruct::LoadAllDetectPlatformSubtypesRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::LoadAllDetectPlatformSubtypesResponse response = m_processCenter.processDetectPlatformSubtypeList(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_DetectPlatformSubtypeList, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case Datastruct::P_DetectPlatformSubtypeDelete: {
				Datastruct::DetectPlatformSubtypeDeleteRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::DetectPlatformSubtypeDeleteResponse response = m_processCenter.processDetectPlatformSubtypeDelete(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_DetectPlatformSubtypeDelete, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case Datastruct::P_DetectPlatformSubtypeModify: {
				Datastruct::DetectPlatformSubtypeModifyRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::DetectPlatformSubtypeModifyResponse response = m_processCenter.processDetectPlatformSubtypeModify(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_DetectPlatformSubtypeModify, JsonWrapper::instance()->wrap(response));
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