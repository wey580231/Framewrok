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

#include <base\util\rutil.h>

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

		// 有关用户
		Datastruct::UserLoginResponse processUserLogin(int clientId,const Datastruct::UserLoginRequest & request);
		Datastruct::UserRegistResponse processUserRegist(int clientId,const Datastruct::UserRegistRequest & request);
		Datastruct::LoadAllUserResponse processUserList(int clientId,const Datastruct::LoadAllUserRequest & request);
		Datastruct::OperateUserResponse processUserOperate(int clientId,const Datastruct::OperateUserRequest & request);

		// 有关任务
		Datastruct::TaskCreateResponse  processTaskCreate(int clientId, const Datastruct::TaskCreateRequest & request);
		Datastruct::LoadAllTaskResponse processTaskList(int clientId, const Datastruct::LoadAllTaskRequest & request);
		Datastruct::TaskByConditionResponse processTaskByCondition(int clientId, const Datastruct::TaskByConditionRequest & request);
		Datastruct::TaskDeleteResponse processTaskDelete(int clientId, const Datastruct::TaskDeleteRequest & request);
		Datastruct::TaskStaticsInfoResponse processTaskStaticsInfo(int clientId, const Datastruct::TaskStaticsInfoRequest & request);
		Datastruct::TaskSimpleResponse processTaskSimple(int clientId, const Datastruct::TaskSimpleRequest & request);
		Datastruct::TaskModifyResponse processTaskModify(int clientId, const Datastruct::TaskModifyRequest & request);

		//有关任务侦查平台
		Datastruct::TaskDetectPlatformCreateResponse  processTaskDetectPlatformCreate(int clientId, const Datastruct::TaskDetectPlatformCreateRequest & request);
		Datastruct::LoadAllTaskDetectPlatformResponse processTaskDetectPlatformList(int clientId, const Datastruct::LoadAllTaskDetectPlatformRequest & request);
		Datastruct::TaskDetectPlatformByConditionResponse processTaskDetectPlatformByCondition(int clientId, const Datastruct::TaskDetectPlatformByConditionRequest & request);
		Datastruct::TaskDetectPlatformDeleteResponse processTaskDetectPlatformDelete(int clientId, const Datastruct::TaskDetectPlatformDeleteRequest & request);
		Datastruct::TaskDetectPlatformModifyResponse processTaskDetectPlatformModify(int clientId, const Datastruct::TaskDetectPlatformModifyRequest & request);

		// 有关任务数据文件
		Datastruct::TaskDataFileCreateResponse  processTaskImageCreate(int clientId, const Datastruct::TaskDataFileCreateRequest & request);
		Datastruct::TaskDataFileCreateResponse  processTaskOriginalXMLCreate(int clientId, const Datastruct::TaskDataFileCreateRequest & request);
		Datastruct::TaskDataFileCreateResponse  processTaskOriginalDataCreate(int clientId, const Datastruct::TaskDataFileCreateRequest & request);

		Datastruct::TaskImageDeleteResponse processTaskImageDelete(int clientId, const Datastruct::TaskImageDeleteRequest & request);
		Datastruct::TaskImageDeleteResponse processTaskOriginalXMLDelete(int clientId, const Datastruct::TaskImageDeleteRequest & request);
		Datastruct::TaskImageDeleteResponse processTaskOriginalDataDelete(int clientId, const Datastruct::TaskImageDeleteRequest & request);


		Datastruct::TaskImageDeleteResponse processTaskImageDelete(int clientId, const Datastruct::TaskImageDeleteRequest & request);
	

		Datastruct::LoadAllTaskImageResponse processTaskImageList(int clientId, const Datastruct::LoadAllTaskImageRequest & request);
		
		// 
// 		void processTaskImageModify(const Datastruct::LoadAllTaskImageRequest & request);
// 		void processTaskOriginalXMLModify(const Datastruct::LoadAllTaskImageRequest & request);
// 		void processTaskOriginalDataModify(const Datastruct::LoadAllTaskImageRequest & request);


	
	

		// 有关值班日志
		Datastruct::DutyRecordCreateResponse  processDutyRecordCreate(int clientId, const Datastruct::DutyRecordCreateRequest & request);
		Datastruct::LoadAllDutyRecordResponse processDutyRecordList(int clientId, const Datastruct::LoadAllDutyRecordRequest & request);
		Datastruct::DutyRecordDeleteResponse  processDutyRecordDelete(int clientId, const Datastruct::DutyRecordDeleteRequest & request);
		Datastruct::DutyRecordModifyResponse  processDutyRecordModify(int clientId, const Datastruct::DutyRecordModifyRequest & request);

		// 有关试验记录
		Datastruct::ExperimentRecordCreateResponse  processExperimentRecordCreate(int clientId, const Datastruct::ExperimentRecordCreateRequest & request);
		Datastruct::LoadAllExperimentRecordsResponse processExperimentRecordList(int clientId, const Datastruct::LoadAllExperimentRecordsRequest & request);
		Datastruct::ExperimentRecordDeleteResponse  processExperimentRecordDelete(int clientId, const Datastruct::ExperimentRecordDeleteRequest & request);
		Datastruct::ExperimentRecordModifyResponse processExperimentRecordModify(int clientId, const Datastruct::ExperimentRecordModifyRequest & request);

		// 有关平台信息
		Datastruct::DetectPlatformCreateResponse  processDetectPlatformCreate(int clientId, const Datastruct::DetectPlatformCreateRequest & request);
		Datastruct::LoadAllDetectPlatformsResponse processDetectPlatformList(int clientId, const Datastruct::LoadAllDetectPlatformsRequest & request);
		Datastruct::DetectPlatformDeleteResponse  processDetectPlatformDelete(int clientId, const Datastruct::DetectPlatformDeleteRequest & request);
		Datastruct::DetectPlatformModifyResponse processDetectPlatformModify(int clientId, const Datastruct::DetectPlatformModifyRequest & request);

		// 有关平台亚型信息
		Datastruct::DetectPlatformSubtypeCreateResponse  processDetectPlatformSubtypeCreate(int clientId, const Datastruct::DetectPlatformSubtypeCreateRequest & request);
		Datastruct::LoadAllDetectPlatformSubtypesResponse processDetectPlatformSubtypeList(int clientId, const Datastruct::LoadAllDetectPlatformSubtypesRequest & request);
		Datastruct::DetectPlatformSubtypeDeleteResponse  processDetectPlatformSubtypeDelete(int clientId, const Datastruct::DetectPlatformSubtypeDeleteRequest & request);
		Datastruct::DetectPlatformSubtypeModifyResponse processDetectPlatformSubtypeModify(int clientId, const Datastruct::DetectPlatformSubtypeModifyRequest & request);

		// 有关目标信息
		Datastruct::TargetCreateResponse  processTargetCreate(int clientId, const Datastruct::TargetCreateRequest & request);
		Datastruct::LoadAllTargetResponse processTargetList(int clientId, const Datastruct::LoadAllTargetRequest & request);
		Datastruct::TargetDeleteResponse  processTargetDelete(int clientId, const Datastruct::TargetDeleteRequest & request);
		Datastruct::TargetModifyResponse processTargetModify(int clientId, const Datastruct::TargetModifyRequest & request);

		// 有关AIS 数据获取
		Datastruct::AISDataCreateResponse  processAISCreate(int clientId, const Datastruct::AISDataCreateRequest & request);
		Datastruct::LoadAllAISDatasResponse processAISDataList(int clientId, const Datastruct::LoadAllAISDataRequest & request);
		Datastruct::AISDataByConditionResponse processAISDataByCondition(int clientId, const Datastruct::AISDataByConditionRequest & request);
		Datastruct::AISDataDeleteResponse processAISDataDelete(int clientId, const Datastruct::AISDataDeleteRequest & request);
		Datastruct::AISDataModifyResponse processAISDataModify(int clientId, const Datastruct::AISDataModifyRequest & request);


	private:


	private:
		Base::Database * m_database;

	};

} //