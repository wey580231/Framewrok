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

	void SignalDispatch::recvDutyRecordCreateResponse(const Datastruct::DutyRecordCreateResponse & response)
	{
		emit respDutyRecordCreateResponse(response);
	}

	void SignalDispatch::recvQueryAllDutyRecordResponse(const Datastruct::LoadAllDutyRecordResponse & response)
	{
		emit respQueryAllDutyRecordResponse(response);
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