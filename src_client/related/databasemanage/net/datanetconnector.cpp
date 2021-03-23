#include "datanetconnector.h"

#include <commondefines/wrapper/jsonwrapper.h>
#include "signaldispatch.h"

#include <QDebug>

using namespace CommonDefines;

namespace Related {

	DataNetConnector * DataNetConnector::m_instance = nullptr;

	DataNetConnector * DataNetConnector::instance()
	{
		if (m_instance == nullptr)
			m_instance = new DataNetConnector();

		return m_instance;
	}

	DataNetConnector::DataNetConnector()
		: NetConnector(Datastruct::Data_Connection)
	{
		connect(this, SIGNAL(newClientData(QByteArray)), this, SLOT(respRectNetData(QByteArray)));
	}

	DataNetConnector::~DataNetConnector()
	{
	}

	void DataNetConnector::write(const Datastruct::UserLoginRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_UserLogin, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::UserRegistRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_UserRegist, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::LoadAllUserRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_UserList, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::OperateUserRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_UserOperate, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TaskCreateRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_CreateTask, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::LoadAllTaskRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskList, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TaskByConditionRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskByCondition, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TaskDeleteRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskDelete, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TaskStaticsInfoRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskStaticsInfo, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TaskModifyRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskModify, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TaskDetectPlatformCreateRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskDetectPlatformCreate, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::LoadAllTaskDetectPlatformRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskDetectPlatformList, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TaskDetectPlatformByConditionRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskDetectPlatformByCondition, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TaskDetectPlatformDeleteRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskDetectPlatformDelete, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TaskDetectPlatformModifyRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskDetectPlatformModify, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TaskDataFileCreateRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskDataFileCreate, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TaskDataFileDeleteRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskDataFileDelete, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::LoadAllTaskImageRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskImageList, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TaskImageDeleteRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskImageDelete, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TaskSimpleRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TaskSimpleInfo, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::DutyRecordCreateRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_CreateDutyRecord, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::LoadAllDutyRecordRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_ListDutyRecords, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::DutyRecordDeleteRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_DeleteDutyRecords, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::DutyRecordModifyRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_ModifyDutyRecord, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::ExperimentRecordCreateRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_CreateExperimentRecord, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::LoadAllExperimentRecordsRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_ListExperimentRecords, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::ExperimentRecordDeleteRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_DeleteExperimentRecord, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::ExperimentRecordModifyRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_ModifyExperimentRecord, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::DetectPlatformCreateRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_DetectPlatformCreate, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::LoadAllDetectPlatformsRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_DetectPlatformList, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::DetectPlatformDeleteRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_DetectPlatformDelete, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::DetectPlatformModifyRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_DetectPlatformModify, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::DetectPlatformSubtypeCreateRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_DetectPlatformSubtypeCreate, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::LoadAllDetectPlatformSubtypesRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_DetectPlatformSubtypeList, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::DetectPlatformSubtypeDeleteRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_DetectPlatformSubtypeDelete, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::DetectPlatformSubtypeModifyRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_DetectPlatformSubtypeModify, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TargetCreateRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TargetCreate, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::LoadAllTargetRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TargetList, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TargetDeleteRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TargetDelete, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::TargetModifyRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_TargetModify, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::AISDataCreateRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_AISCreate, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::LoadAllAISDataRequest & request)
	{
		QByteArray array = makePacket(Datastruct::P_AISList, CommonDefines::JsonWrapper::instance()->wrap(request));
		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::AISDataDeleteRequest & request)
	{
// 		QByteArray array = makePacket(Datastruct::P_AISDelete, CommonDefines::JsonWrapper::instance()->wrap(request));
// 		sendData(array);
	}

	void DataNetConnector::write(const Datastruct::AISDataModifyRequest & request)
	{
// 		QByteArray array = makePacket(Datastruct::P_AISModify, CommonDefines::JsonWrapper::instance()->wrap(request));
// 		sendData(array);
	}

	QByteArray DataNetConnector::makePacket(Datastruct::PacketType type, QByteArray & body)
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

	/*!
	 * @brief 处理解析后的网络数据
	 * @param array 服务器端反馈数据
	 */
	void DataNetConnector::respRectNetData(QByteArray array)
	{
		Datastruct::PacketHead head;
		memcpy((char *)&head, array.data(), sizeof(Datastruct::PacketHead));

		QByteArray jsonData(array.data() + sizeof(head), array.size() - sizeof(Datastruct::PacketHead) - sizeof(Datastruct::PacketTail));

		switch (head.m_packetType)
		{
			/*****************  有关用户  *****************************/
			case Datastruct::P_UserLogin: {
				Datastruct::UserLoginResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvUserLoginResponse(response);
				}
			}
				break;
			case Datastruct::P_UserRegist: {
				Datastruct::UserRegistResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvUseRegistResponse(response);
				}
			}
				break;
			case Datastruct::P_UserList: {
				Datastruct::LoadAllUserResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvQueryUserListResponse(response);
				}
			}
			 break;
			case Datastruct::P_UserOperate: {
				Datastruct::OperateUserResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvOperateUserResponse(response);
				}
			}
				break;

				/*****************  有关任务  *****************************/ 
			case  Datastruct::P_CreateTask: {
				Datastruct::TaskCreateResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTaskCreateResponse(response);
				}
			}
				break;
			case  Datastruct::P_TaskList: {
				Datastruct::LoadAllTaskResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvQueryAllTaskResponse(response);
				}
			}
				break;
			case  Datastruct::P_TaskByCondition: {
				Datastruct::TaskByConditionResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTaskByConditionResponse(response);
				}
			}
			 break;
			case  Datastruct::P_TaskDelete: {
				Datastruct::TaskDeleteResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTaskDeleteResponse(response);
				}
			}
				break;
			case  Datastruct::P_TaskStaticsInfo: {
				Datastruct::TaskStaticsInfoResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTaskStaticsInfoResponse(response);
				}
			}
				 break;
			case  Datastruct::P_TaskSimpleInfo: {
				Datastruct::TaskSimpleResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTaskSimpleResponse(response);
				}
			}
				break;
			case  Datastruct::P_TaskFullInfo: {

			}
				break;
			case  Datastruct::P_TaskModify: {
				Datastruct::TaskModifyResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTaskModifyResponse(response);
				}
			}
				break;

				/*****************  任务侦测平台亚型  *****************************/ 
			case  Datastruct::P_TaskDetectPlatformCreate: {
				Datastruct::TaskDetectPlatformCreateResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTaskDetectPlatformCreateResponse(response);
				}
			}
				break;
			case  Datastruct::P_TaskDetectPlatformList: {
				Datastruct::LoadAllTaskDetectPlatformResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvQueryAllTaskDetectPlatformResponse(response);
				}
			}
				break;
			case  Datastruct::P_TaskDetectPlatformByCondition: {
				Datastruct::TaskDetectPlatformByConditionResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTaskDetectPlatformByConditionResponse(response);
				}
			}
				break;
			case  Datastruct::P_TaskDetectPlatformDelete: {
				Datastruct::TaskDetectPlatformDeleteResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTaskDetectPlatformDeleteResponse(response);
				}
			}
				break;
			case  Datastruct::P_TaskDetectPlatformModify: {
				Datastruct::TaskDetectPlatformModifyResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTaskDetectPlatformModifyResponse(response);
				}
			}
				break;

			/*****************  任务数据文件  *****************************/
			case  Datastruct::P_TaskDataFileCreate: {
				Datastruct::TaskDataFileCreateResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTaskDataFileCreateResponse(response);
				}
			}
			  break;
			case Datastruct::P_TaskDataFileDelete: {

			}
												   break;
			/*****************  任务试验图片资源  *****************************/

			case  Datastruct::P_TaskImageList: {
				Datastruct::LoadAllTaskImageResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvQueryAllTaskImageResponse(response);
				}
			}
				break;

			case  Datastruct::P_TaskImageDelete: {
				Datastruct::TaskImageDeleteResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTaskImageDeleteResponse(response);
				}
			}
				break;

				/*****************    *****************************/
			case  Datastruct::P_CreateDutyRecord: {
				Datastruct::DutyRecordCreateResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvDutyRecordCreateResponse(response);
				}
			}
				break;
			case  Datastruct::P_ListDutyRecords: {
				Datastruct::LoadAllDutyRecordResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvQueryAllDutyRecordResponse(response);
				}
			}
				break;
			case  Datastruct::P_DeleteDutyRecords: {
				Datastruct::DutyRecordDeleteResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvDutyRecordDeleteResponse(response);
				}
			}
				break;
			case   Datastruct::P_ModifyDutyRecord: {
				Datastruct::DutyRecordModifyResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvDutyRecordModifyResponse(response);
				}
			}
				break;
			case  Datastruct::P_CreateExperimentRecord: {
				Datastruct::ExperimentRecordCreateResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvExperimentRecordCreateResponse(response);
				}
			}
				break;
			case  Datastruct::P_ListExperimentRecords: {
				Datastruct::LoadAllExperimentRecordsResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvQueryAllExperimentRecordResponse(response);
				}
			}
				break;
			case  Datastruct::P_DeleteExperimentRecord: {
				Datastruct::ExperimentRecordDeleteResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvExperimentRecordDeleteResponse(response);
				}
			}
				break;
			case   Datastruct::P_ModifyExperimentRecord: {
				Datastruct::ExperimentRecordModifyResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvExperimentRecordModifyResponse(response);
				}
			}
				break;


			case  Datastruct::P_DetectPlatformCreate: {
				Datastruct::DetectPlatformCreateResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvDetectPlatformCreateResponse(response);
				}
			}
				break;
			case  Datastruct::P_DetectPlatformList: {
				Datastruct::LoadAllDetectPlatformsResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvQueryAllDetectPlatformsResponse(response);
				}
			}
				break;
			case  Datastruct::P_DetectPlatformDelete: {
				Datastruct::DetectPlatformDeleteResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvDetectPlatformDeleteResponse(response);
				}
			}
				break;
			case  Datastruct::P_DetectPlatformModify: {
				Datastruct::DetectPlatformModifyResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvDetectPlatformModifyResponse(response);
				}
			}
				break;
			case  Datastruct::P_DetectPlatformSubtypeCreate: {
				Datastruct::DetectPlatformSubtypeCreateResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvDetectPlatformSubtypeCreateResponse(response);
				}
			}
				break;
			case  Datastruct::P_DetectPlatformSubtypeList: {
				Datastruct::LoadAllDetectPlatformSubtypesResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvQueryAllDetectPlatformSubtypesResponse(response);
				}
			}
				break;
			case  Datastruct::P_DetectPlatformSubtypeDelete: {
				Datastruct::DetectPlatformSubtypeDeleteResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvDetectPlatformSubtypeDeleteResponse(response);
				}
			}
				break;
			case  Datastruct::P_DetectPlatformSubtypeModify: {
				Datastruct::DetectPlatformSubtypeModifyResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvDetectPlatformSubtypeModifyResponse(response);
				}
			}
				break;

			// 目标信息
			case  Datastruct::P_TargetCreate: {
				Datastruct::TargetCreateResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTargetCreateResponse(response);
				}
			}
				break;
			case  Datastruct::P_TargetList: {
				Datastruct::LoadAllTargetResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvQueryAllTargetResponse(response);
				}
			}
				break;
			case  Datastruct::P_TargetDelete: {
				Datastruct::TargetDeleteResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTargetDeleteResponse(response);
				}
			}
				break;
			case  Datastruct::P_TargetModify: {
				Datastruct::TargetModifyResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvTargetModifyResponse(response);
				}
			}
				break;
			
				//AIS数据
			case  Datastruct::P_AISCreate: {
				Datastruct::AISDataCreateResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvAISDataCreateResponse(response);
				}
			}
				break;
			case  Datastruct::P_AISList: {
				Datastruct::LoadAllAISDatasResponse response;
				if (CommonDefines::JsonWrapper::instance()->unrap(jsonData, response)) {
					SignalDispatch::instance()->recvQueryAllAISDataResponse(response);
				}
			}
				break;
			case  Datastruct::P_AISDelete: {

			}
				break;
			case  Datastruct::P_AISModify: {

			}
				break;

	

			default:
				break;
		}
	}

	void DataNetConnector::processNetData(QByteArray & data)
	{
		emit newClientData(data);
	}

} //namespace Related 