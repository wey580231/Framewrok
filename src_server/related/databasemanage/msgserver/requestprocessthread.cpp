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

			/************************ 任务  *********************************/
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
				Datastruct::TaskByConditionRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TaskByConditionResponse response = m_processCenter.processTaskByCondition(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TaskByCondition, JsonWrapper::instance()->wrap(response));
				}
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
				Datastruct::TaskStaticsInfoRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TaskStaticsInfoResponse response = m_processCenter.processTaskStaticsInfo(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TaskStaticsInfo, JsonWrapper::instance()->wrap(response));
				}
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
			case  Datastruct::P_TaskModify: {
				Datastruct::TaskModifyRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TaskModifyResponse response = m_processCenter.processTaskModify(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TaskModify, JsonWrapper::instance()->wrap(response));
				}
			}
				break;

			/************************ 任务侦测平台亚型 *********************************/
			case  Datastruct::P_TaskDetectPlatformCreate: {
				Datastruct::TaskDetectPlatformCreateRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TaskDetectPlatformCreateResponse response = m_processCenter.processTaskDetectPlatformCreate(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TaskDetectPlatformCreate, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_TaskDetectPlatformList: {
				Datastruct::LoadAllTaskDetectPlatformRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::LoadAllTaskDetectPlatformResponse response = m_processCenter.processTaskDetectPlatformList(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TaskDetectPlatformList, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_TaskDetectPlatformByCondition: {
				Datastruct::TaskDetectPlatformByConditionRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TaskDetectPlatformByConditionResponse response = m_processCenter.processTaskDetectPlatformByCondition(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TaskDetectPlatformByCondition, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_TaskDetectPlatformDelete: {
				Datastruct::TaskDetectPlatformDeleteRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TaskDetectPlatformDeleteResponse response = m_processCenter.processTaskDetectPlatformDelete(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TaskDetectPlatformDelete, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_TaskDetectPlatformModify: {
				Datastruct::TaskDetectPlatformModifyRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TaskDetectPlatformModifyResponse response = m_processCenter.processTaskDetectPlatformModify(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TaskDetectPlatformModify, JsonWrapper::instance()->wrap(response));
				}
			}
				break;

			/************************ 任务数据文件 *********************************/
			// 任务图片资源
			case  Datastruct::P_TaskDataFileCreate: {
				Datastruct::TaskDataFileCreateRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TaskDataFileCreateResponse response;
					if (request.m_suffix == QStringLiteral("png") || request.m_suffix == QStringLiteral("jpg")) {
						response = m_processCenter.processTaskImageCreate(unit->m_clientId, request);
					}
					else if(request.m_suffix == QStringLiteral("xml"))
					{
						response = m_processCenter.processTaskOriginalXMLCreate(unit->m_clientId, request);

					}
					else if (request.m_suffix == QStringLiteral("dat"))
					{
						response = m_processCenter.processTaskOriginalDataCreate(unit->m_clientId, request);
					}
					runit->m_resposneData = makePacket(Datastruct::P_TaskDataFileCreate, JsonWrapper::instance()->wrap(response));
				}
			}
				break;


			case  Datastruct::P_TaskImageList : {
				Datastruct::LoadAllTaskImageRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::LoadAllTaskImageResponse response = m_processCenter.processTaskImageList(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TaskImageList, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_TaskImageDelete : {
				Datastruct::TaskImageDeleteRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TaskImageDeleteResponse response = m_processCenter.processTaskImageDelete(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TaskImageDelete, JsonWrapper::instance()->wrap(response));
				}
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

			/******************************     数据       **********************************************/
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
	
			/******************************     目标数据       **********************************************/
			case  Datastruct::P_TargetCreate: {
				Datastruct::TargetCreateRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TargetCreateResponse response = m_processCenter.processTargetCreate(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TargetCreate, JsonWrapper::instance()->wrap(response));
				}
			} 
				break;
			case  Datastruct::P_TargetList: {
				Datastruct::LoadAllTargetRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::LoadAllTargetResponse response = m_processCenter.processTargetList(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TargetList, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_TargetByCondition: {

			}
				break;
			case  Datastruct::P_TargetDelete: {
				Datastruct::TargetDeleteRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TargetDeleteResponse response = m_processCenter.processTargetDelete(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TargetCreate, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_TargetModify: {
				Datastruct::TargetModifyRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::TargetModifyResponse response = m_processCenter.processTargetModify(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_TargetModify, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_TargetSimpleInfo: {

			}
				break;
			case  Datastruct::P_TargetFullInfo: {

			}
				break;

			/******************************     AIS数据       **********************************************/
			case  Datastruct::P_AISCreate: {
				Datastruct::AISDataCreateRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::AISDataCreateResponse response = m_processCenter.processAISCreate(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_AISCreate, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_AISList: {
				Datastruct::LoadAllAISDataRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::LoadAllAISDatasResponse response = m_processCenter.processAISDataList(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_AISList, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_AISByCondition: {
				Datastruct::AISDataByConditionRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::AISDataByConditionResponse response = m_processCenter.processAISDataByCondition(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_AISByCondition, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_AISDelete: {
				Datastruct::AISDataDeleteRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::AISDataDeleteResponse response = m_processCenter.processAISDataDelete(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_AISDelete, JsonWrapper::instance()->wrap(response));
				}
			}
				break;
			case  Datastruct::P_AISModify: {
				Datastruct::AISDataModifyRequest request;
				if (JsonWrapper::instance()->unrap(jsonData, request)) {
					Datastruct::AISDataModifyResponse response = m_processCenter.processAISDataModify(unit->m_clientId, request);
					runit->m_resposneData = makePacket(Datastruct::P_AISModify, JsonWrapper::instance()->wrap(response));
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