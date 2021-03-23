#include "signaldispatch.h"

#include <QDebug>

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

	void SignalDispatch::recvTaskByConditionResponse(const Datastruct::TaskByConditionResponse & response)
	{
		emit respTaskByConditionResponse(response);
	}

	void SignalDispatch::recvTaskDeleteResponse(const Datastruct::TaskDeleteResponse & response)
	{
		emit respTaskeDleteResponse(response);
	}

	void SignalDispatch::recvTaskStaticsInfoResponse(const Datastruct::TaskStaticsInfoResponse & response)
	{
		emit respTaskStaticsInfoResponse(response);
	}

	void SignalDispatch::recvTaskSimpleResponse(const Datastruct::TaskSimpleResponse & response)
	{
		emit respTaskSimpleResponse(response);
	}

	void SignalDispatch::recvTaskModifyResponse(const Datastruct::TaskModifyResponse & response)
	{
		emit respTaskModifyResponse(response);
	}

	void SignalDispatch::recvTaskDetectPlatformCreateResponse(const Datastruct::TaskDetectPlatformCreateResponse & response)
	{
		emit respTaskDetectPlatformCreateResponse(response);
	}

	void SignalDispatch::recvQueryAllTaskDetectPlatformResponse(const Datastruct::LoadAllTaskDetectPlatformResponse & response)
	{
		emit respQueryAllTaskDetectPlatformResponse(response);
	}

	void SignalDispatch::recvTaskDetectPlatformByConditionResponse(const Datastruct::TaskDetectPlatformByConditionResponse & response)
	{
		emit respTaskDetectPlatformByConditionResponse(response);
	}

	void SignalDispatch::recvTaskDetectPlatformDeleteResponse(const Datastruct::TaskDetectPlatformDeleteResponse & response)
	{
		emit respTaskDetectPlatformDeleteResponse(response);
	}

	void SignalDispatch::recvTaskDetectPlatformModifyResponse(const Datastruct::TaskDetectPlatformModifyResponse & response)
	{
		emit respTaskDetectPlatformModifyResponse(response);
	}

	void SignalDispatch::recvTaskDataFileCreateResponse(const Datastruct::TaskDataFileCreateResponse & response)
	{
		emit respTaskDataFileCreateResponse(response);
	}

	void SignalDispatch::recvQueryAllTaskImageResponse(const Datastruct::LoadAllTaskImageResponse & response)
	{
		emit respQueryAllTaskImageResponse(response);
	}

	void SignalDispatch::recvTaskImageDeleteResponse(const Datastruct::TaskImageDeleteResponse & response)
	{
		emit respTaskImageDeleteResponse(response);
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

	void SignalDispatch::recvTargetCreateResponse(const Datastruct::TargetCreateResponse & response)
	{
		emit respTargetCreateResponse(response);
	}

	void SignalDispatch::recvQueryAllTargetResponse(const Datastruct::LoadAllTargetResponse & response)
	{
		emit respQueryAllTargetResponse(response);
	}

	void SignalDispatch::recvTargetDeleteResponse(const Datastruct::TargetDeleteResponse & response)
	{
		emit respTargetDeleteResponse(response);
	}

	void SignalDispatch::recvTargetModifyResponse(const Datastruct::TargetModifyResponse & response)
	{
		emit respTargetModifyResponse(response);
	}

	void SignalDispatch::recvAISDataCreateResponse(const Datastruct::AISDataCreateResponse & response)
	{
		emit respAISDataCreateResponse(response);
	}

	void SignalDispatch::recvQueryAllAISDataResponse(const Datastruct::LoadAllAISDatasResponse & response)
	{
		emit respQueryAllAISDataResponse(response);
	}

	void SignalDispatch::recvAISDataDeleteResponse(const Datastruct::AISDataDeleteResponse & response)
	{
		emit respAISDataDeleteResponse(response);
	}

	void SignalDispatch::recvAISDataModifyResponse(const Datastruct::AISDataModifyResponse & response)
	{
		emit respAISDataModifyResponse(response);
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