/*!
 * @brief     处理各个网络请求业务，并返回执行结果   
 * @author    wey
 * @version   1.0
 * @date      2021.01.23 10:06:22
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

#include <commondefines/protocol.h>
#include <base\common\sql\databasemanager.h>

namespace Related {

	class DataProcessCenter : public QObject
	{
		Q_OBJECT

	public:
		DataProcessCenter(QObject *parent = nullptr);
		~DataProcessCenter();
		
		/*! 
		 * @brief 绑定数据库连接【每个线程的数据库连接只能在当前线程中使用】
		 * @param db 数据库连接句柄
		 */
		void bindDatabase(Base::Database * db);

		Datastruct::UserLoginResponse processUserLogin(int clientId,const Datastruct::UserLoginRequest & request);
		Datastruct::UserRegistResponse processUserRegist(int clientId,const Datastruct::UserRegistRequest & request);
		Datastruct::LoadAllUserResponse processUserList(int clientId,const Datastruct::LoadAllUserRequest & request);
		Datastruct::OperateUserResponse processUserOperate(int clientId,const Datastruct::OperateUserRequest & request);

		// 有关任务
		Datastruct::TaskCreateResponse  processTaskCreate(int clientId, const Datastruct::TaskCreateRequest & request);
		Datastruct::LoadAllTaskResponse processTaskList(int clientId, const Datastruct::LoadAllTaskRequest & request);
		Datastruct::TaskDeleteResponse processTaskDelete(int clientId, const Datastruct::TaskDeleteRequest & request);
		Datastruct::TaskSimpleResponse processTaskSimple(int clientId, const Datastruct::TaskSimpleRequest & request);
		//值班日志
		Datastruct::DutyRecordCreateResponse  processDutyRecordCreate(int clientId, const Datastruct::DutyRecordCreateRequest & request);
		Datastruct::LoadAllDutyRecordResponse processDutyRecordList(int clientId, const Datastruct::LoadAllDutyRecordRequest & request);
		Datastruct::DutyRecordDeleteResponse processDutyRecordDelete(int clientId, const Datastruct::DutyRecordDeleteRequest & request);

		//试验记录
		Datastruct::ExperimentRecordCreateResponse  processExperimentRecordCreate(int clientId, const Datastruct::ExperimentRecordCreateRequest & request);
		Datastruct::LoadAllExperimentRecordsResponse  processExperimentRecordList(int clientId, const Datastruct::LoadAllExperimentRecordsRequest & request);
		Datastruct::ExperimentRecordDeleteResponse  processExperimentRecordDelete(int clientId, const Datastruct::ExperimentRecordDeleteRequest & request);
	private:
		Base::Database * m_database;

	};

} //