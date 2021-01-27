/*!
 * @brief     信号中转站
 * @details   负责在网络处理和各个页面之间建立信号连接,建立统一的信号连接接口
 * @author    wey
 * @version   1.0
 * @date      2021.01.22 08:05:25
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

#include <commondefines/protocol.h>

namespace Related {

	class SignalDispatch : public QObject
	{
		Q_OBJECT

	public:
		static SignalDispatch * instance();
		~SignalDispatch();

		void recvUserLoginResponse(const Datastruct::UserLoginResponse & response);
		void recvUseRegistResponse(const Datastruct::UserRegistResponse & response);
		void recvQueryUserListResponse(const Datastruct::LoadAllUserResponse & response);
		void recvOperateUserResponse(const Datastruct::OperateUserResponse & response);
		//任务有关
		void recvTaskCreateResponse(const Datastruct::TaskCreateResponse & response);
		void recvQueryAllTaskResponse(const Datastruct::LoadAllTaskResponse & response);
		void recvTaskDeleteResponse(const Datastruct::TaskDeleteResponse & response);
		void recvTaskSimpleResponse(const Datastruct::TaskSimpleResponse & response);
		
		void recvDutyRecordCreateResponse(const Datastruct::DutyRecordCreateResponse & response);
		void recvQueryAllDutyRecordResponse(const Datastruct::LoadAllDutyRecordResponse & response);
		void recvDutyRecordDeleteResponse(const Datastruct::DutyRecordDeleteResponse & response);

		void recvExperimentRecordCreateResponse(const Datastruct::ExperimentRecordCreateResponse & response);
		void recvQueryAllExperimentRecordResponse(const Datastruct::LoadAllExperimentRecordsResponse & response);
		void recvExperimentRecordDeleteResponse(const Datastruct::ExperimentRecordDeleteResponse & response);

	signals:
		void respUserLoginResponse(const Datastruct::UserLoginResponse & response);
		void respUserRegistResponse(const Datastruct::UserRegistResponse & response);
		void respQueryUserListResponse(const Datastruct::LoadAllUserResponse & response);
		void resOperateUserResponse(const Datastruct::OperateUserResponse & response);

		void respTaskCreateResponse(const Datastruct::TaskCreateResponse & response);
		void respQueryAllTaskResponse(const Datastruct::LoadAllTaskResponse & response);
		void respTaskeDleteResponse(const Datastruct::TaskDeleteResponse & response);
		void respTaskSimpleResponse(const Datastruct::TaskSimpleResponse & response);

 		void respDutyRecordCreateResponse(const Datastruct::DutyRecordCreateResponse & response);
		void respQueryAllDutyRecordResponse(const Datastruct::LoadAllDutyRecordResponse & response);
		void respDutyRecordDeleteResponse(const Datastruct::DutyRecordDeleteResponse & response);

		void respExperimentRecordCreateResponse(const Datastruct::ExperimentRecordCreateResponse & response);
		void respQueryAllExperimentRecordResponse(const Datastruct::LoadAllExperimentRecordsResponse & response);
		void respExperimentRecordDeleteResponse(const Datastruct::ExperimentRecordDeleteResponse & response);
	private:
		SignalDispatch(QObject *parent = nullptr);


	private:
		static SignalDispatch * m_instance;

		friend class JsonWrapper;
	};

} //namespace Related 