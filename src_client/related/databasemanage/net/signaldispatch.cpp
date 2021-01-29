#include "signaldispatch.h"

namespace Related {

	SignalDispatch * SignalDispatch::m_instance = nullptr;

	SignalDispatch::SignalDispatch(QObject *parent)
		: QObject(parent)
	{
	}

	void SignalDispatch::recvUserLoginResponse(const Datastruct::UserLoginResponse & response)
	{
		emit respUserLoginResponse(response);
	}

	void SignalDispatch::recvUseRegistResponse(const Datastruct::UserRegistResponse & response)
	{
		emit respUserRegistResponse(response);
	}

	void SignalDispatch::recvQueryUserListResponse(const Datastruct::LoadAllUserResponse & response)
	{
		emit respQueryUserListResponse(response);
	}

	void SignalDispatch::recvOperateUserResponse(const Datastruct::OperateUserResponse & response)
	{
		emit resOperateUserResponse(response);
	}

	void SignalDispatch::recvTaskCreateResponse(const Datastruct::TaskCreateResponse & response)
	{
		emit respTaskCreateResponse(response);
	}

	void SignalDispatch::recvQueryAllTaskResponse(const Datastruct::LoadAllTaskResponse & response)
	{
		emit respQueryAllTaskResponse(response);
	}

	void SignalDispatch::recvTaskDeleteResponse(const Datastruct::TaskDeleteResponse & response)
	{
		emit respTaskeDleteResponse(response);
	}

	void SignalDispatch::recvTaskSimpleResponse(const Datastruct::TaskSimpleResponse & response)
	{
		emit respTaskSimpleResponse(response);
	}

	void SignalDispatch::recvDutyRecordCreateResponse(const Datastruct::DutyRecordCreateResponse & response)
	{
		emit respDutyRecordCreateResponse(response);
	}

	void SignalDispatch::recvQueryAllDutyRecordResponse(const Datastruct::LoadAllDutyRecordResponse & response)
	{
		emit respQueryAllDutyRecordResponse(response);

	}
	void SignalDispatch::recvDutyRecordDeleteResponse(const Datastruct::DutyRecordDeleteResponse & response)
	{
		emit respDutyRecordDeleteResponse(response);
	}

	void SignalDispatch::recvDutyRecordModifyResponse(const Datastruct::DutyRecordModifyResponse & response)
	{
		emit respDutyRecordModifyResponse(response);
	}

	void SignalDispatch::recvExperimentRecordCreateResponse(const Datastruct::ExperimentRecordCreateResponse & response)
	{
		emit respExperimentRecordCreateResponse(response);
	}

	void SignalDispatch::recvQueryAllExperimentRecordResponse(const Datastruct::LoadAllExperimentRecordsResponse & response)
	{
		emit respQueryAllExperimentRecordResponse(response);
	}

	void SignalDispatch::recvExperimentRecordDeleteResponse(const Datastruct::ExperimentRecordDeleteResponse & response)
	{
		emit respExperimentRecordDeleteResponse(response);
	}

	void SignalDispatch::recvExperimentRecordModifyResponse(const Datastruct::ExperimentRecordModifyResponse & response)
	{
		emit respExperimentRecordModifyResponse(response);
	}

	void SignalDispatch::recvDetectPlatformCreateResponse(const Datastruct::DetectPlatformCreateResponse & response)
	{
		emit respDetectPlatformCreateResponse(response);
	}

	void SignalDispatch::recvQueryAllDetectPlatformsResponse(const Datastruct::LoadAllDetectPlatformsResponse & response)
	{
		emit respQueryAllDetectPlatformsResponse(response);
	}

	void SignalDispatch::recvDetectPlatformDeleteResponse(const Datastruct::DetectPlatformDeleteResponse & response)
	{
		emit respDetectPlatformDeleteResponse(response);
	}

	void SignalDispatch::recvDetectPlatformModifyResponse(const Datastruct::DetectPlatformModifyResponse & response)
	{
		emit respDetectPlatformModifyResponse(response);
	}

	void SignalDispatch::recvDetectPlatformSubtypeCreateResponse(const Datastruct::DetectPlatformSubtypeCreateResponse & response)
	{
		emit respDetectPlatformSubtypeCreateResponse(response);
	}

	void SignalDispatch::recvQueryAllDetectPlatformSubtypesResponse(const Datastruct::LoadAllDetectPlatformSubtypesResponse & response)
	{
		emit respQueryAllDetectPlatformSubtypesResponse(response);
	}

	void SignalDispatch::recvDetectPlatformSubtypeDeleteResponse(const Datastruct::DetectPlatformSubtypeDeleteResponse & response)
	{
		emit respDetectPlatformSubtypeDeleteResponse(response);
	}

	void SignalDispatch::recvDetectPlatformSubtypeModifyResponse(const Datastruct::DetectPlatformSubtypeModifyResponse & response)
	{
		emit respDetectPlatformSubtypeModifyResponse(response);
	}

	SignalDispatch * SignalDispatch::instance()
	{
		if (m_instance == nullptr)
			m_instance = new SignalDispatch();
		return m_instance;
	}

	SignalDispatch::~SignalDispatch()
	{
	}

} //namespace Related 