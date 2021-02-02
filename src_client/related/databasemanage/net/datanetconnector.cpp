#include "datanetconnector.h"

#include <commondefines/wrapper/jsonwrapper.h>
#include "signaldispatch.h"

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
		: NetConnector()
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

			default:
				break;
		}
	}

	void DataNetConnector::processNetData(QByteArray & data)
	{
		emit newClientData(data);
	}

} //namespace Related 