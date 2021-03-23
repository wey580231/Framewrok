/*!
 * @brief     ������ת����JSON��ʽ������������ݰ�   
 * @author    wey
 * @version   1.0
 * @date      2021.01.21 15:09:58
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <functional>

#include <QObject>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "../protocol.h"
#include "../commondefines_global.h"
#include "jsonkey.h"

namespace CommonDefines {

	class COMMONDEFINESHARED_EXPORT JsonWrapper : public QObject
	{
		Q_OBJECT

	public:
		static JsonWrapper * instance();
		~JsonWrapper();

		// �й��û�
		QByteArray wrap(const Datastruct::UserLoginRequest & request);
		bool unrap(const QByteArray & data, Datastruct::UserLoginRequest & request); 

		QByteArray wrap(const Datastruct::UserLoginResponse & response);
		bool unrap(const QByteArray & data, Datastruct::UserLoginResponse & response);

		QByteArray wrap(const Datastruct::UserRegistRequest & request);
		bool unrap(const QByteArray & data, Datastruct::UserRegistRequest & request);

		QByteArray wrap(const Datastruct::UserRegistResponse & response);
		bool unrap(const QByteArray & data, Datastruct::UserRegistResponse & response); 
		
		QByteArray wrap(const Datastruct::LoadAllUserRequest & request);
		bool unrap(const QByteArray & data, Datastruct::LoadAllUserRequest & request);

		QByteArray wrap(const Datastruct::LoadAllUserResponse & response);
		bool unrap(const QByteArray & data, Datastruct::LoadAllUserResponse & response);

		QByteArray wrap(const Datastruct::OperateUserRequest & request);
		bool unrap(const QByteArray & data, Datastruct::OperateUserRequest & request);

		QByteArray wrap(const Datastruct::OperateUserResponse & response);
		bool unrap(const QByteArray & data, Datastruct::OperateUserResponse & response);
		
		//TODO 20210125 �й�����
		QByteArray wrap(const Datastruct::TaskCreateRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TaskCreateRequest & request);

		QByteArray wrap(const Datastruct::TaskCreateResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TaskCreateResponse & response);

		QByteArray wrap(const Datastruct::LoadAllTaskRequest & request);
		bool unrap(const QByteArray & data, Datastruct::LoadAllTaskRequest & request);

		QByteArray wrap(const Datastruct::LoadAllTaskResponse & response);
		bool unrap(const QByteArray & data, Datastruct::LoadAllTaskResponse & response);

		QByteArray wrap(const Datastruct::TaskByConditionRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TaskByConditionRequest & request);

		QByteArray wrap(const Datastruct::TaskByConditionResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TaskByConditionResponse & response);

		QByteArray wrap(const Datastruct::TaskDeleteRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TaskDeleteRequest & request);

		QByteArray wrap(const Datastruct::TaskDeleteResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TaskDeleteResponse & response);

		QByteArray wrap(const Datastruct::TaskStaticsInfoRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TaskStaticsInfoRequest & request);

		QByteArray wrap(const Datastruct::TaskStaticsInfoResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TaskStaticsInfoResponse & response);

		QByteArray wrap(const Datastruct::TaskSimpleRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TaskSimpleRequest & request);

		QByteArray wrap(const Datastruct::TaskSimpleResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TaskSimpleResponse & response);

		QByteArray wrap(const Datastruct::TaskModifyRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TaskModifyRequest & request);

		QByteArray wrap(const Datastruct::TaskModifyResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TaskModifyResponse & response);

		//�й��������ƽ̨
		QByteArray wrap(const Datastruct::TaskDetectPlatformCreateRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TaskDetectPlatformCreateRequest & request);

		QByteArray wrap(const Datastruct::TaskDetectPlatformCreateResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TaskDetectPlatformCreateResponse & response);

		QByteArray wrap(const Datastruct::LoadAllTaskDetectPlatformRequest & request);
		bool unrap(const QByteArray & data, Datastruct::LoadAllTaskDetectPlatformRequest & request);

		QByteArray wrap(const Datastruct::LoadAllTaskDetectPlatformResponse & response);
		bool unrap(const QByteArray & data, Datastruct::LoadAllTaskDetectPlatformResponse & response);

		QByteArray wrap(const Datastruct::TaskDetectPlatformByConditionRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TaskDetectPlatformByConditionRequest & request);

		QByteArray wrap(const Datastruct::TaskDetectPlatformByConditionResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TaskDetectPlatformByConditionResponse & response);

		QByteArray wrap(const Datastruct::TaskDetectPlatformDeleteRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TaskDetectPlatformDeleteRequest & request);

		QByteArray wrap(const Datastruct::TaskDetectPlatformDeleteResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TaskDetectPlatformDeleteResponse & response);

		QByteArray wrap(const Datastruct::TaskDetectPlatformModifyRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TaskDetectPlatformModifyRequest & request);

		QByteArray wrap(const Datastruct::TaskDetectPlatformModifyResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TaskDetectPlatformModifyResponse & response);

		// �й����������ļ�
		QByteArray wrap(const Datastruct::TaskDataFileCreateRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TaskDataFileCreateRequest & request);

		QByteArray wrap(const Datastruct::TaskDataFileCreateResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TaskDataFileCreateResponse & response);

		QByteArray wrap(const Datastruct::TaskDataFileDeleteRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TaskDataFileDeleteRequest & request);

		QByteArray wrap(const Datastruct::TaskDataFileDeleteResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TaskDataFileDeleteResponse & response);


		QByteArray wrap(const Datastruct::LoadAllTaskImageRequest & request);
		bool unrap(const QByteArray & data, Datastruct::LoadAllTaskImageRequest & request);

		QByteArray wrap(const Datastruct::LoadAllTaskImageResponse & response);
		bool unrap(const QByteArray & data, Datastruct::LoadAllTaskImageResponse & response);

		QByteArray wrap(const Datastruct::TaskImageDeleteRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TaskImageDeleteRequest & request);

		QByteArray wrap(const Datastruct::TaskImageDeleteResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TaskImageDeleteResponse & response);

		//TODO 20210125 �й�ֵ����־
		QByteArray wrap(const Datastruct::DutyRecordCreateRequest & request);
		bool unrap(const QByteArray & data, Datastruct::DutyRecordCreateRequest & request);

		QByteArray wrap(const Datastruct::DutyRecordCreateResponse & response);
		bool unrap(const QByteArray & data, Datastruct::DutyRecordCreateResponse & response);

		QByteArray wrap(const Datastruct::LoadAllDutyRecordRequest & request);
		bool unrap(const QByteArray & data, Datastruct::LoadAllDutyRecordRequest & request);

		QByteArray wrap(const Datastruct::LoadAllDutyRecordResponse & response);
		bool unrap(const QByteArray & data, Datastruct::LoadAllDutyRecordResponse & response);

		QByteArray wrap(const Datastruct::DutyRecordDeleteRequest & request);
		bool unrap(const QByteArray & data, Datastruct::DutyRecordDeleteRequest & request);

		QByteArray wrap(const Datastruct::DutyRecordDeleteResponse & response);
		bool unrap(const QByteArray & data, Datastruct::DutyRecordDeleteResponse & response);

		QByteArray wrap(const Datastruct::DutyRecordModifyRequest & request);
		bool unrap(const QByteArray & data, Datastruct::DutyRecordModifyRequest & request);

		QByteArray wrap(const Datastruct::DutyRecordModifyResponse & response);
		bool unrap(const QByteArray & data, Datastruct::DutyRecordModifyResponse & response);

		//TODO 20210125 23:15:29 �й������¼
		QByteArray wrap(const Datastruct::ExperimentRecordCreateRequest & request);
		bool unrap(const QByteArray & data, Datastruct::ExperimentRecordCreateRequest & request);

		QByteArray wrap(const Datastruct::ExperimentRecordCreateResponse & response);
		bool unrap(const QByteArray & data, Datastruct::ExperimentRecordCreateResponse & response);

		QByteArray wrap(const Datastruct::LoadAllExperimentRecordsRequest & request);
		bool unrap(const QByteArray & data, Datastruct::LoadAllExperimentRecordsRequest & request);

		QByteArray wrap(const Datastruct::LoadAllExperimentRecordsResponse & response);
		bool unrap(const QByteArray & data, Datastruct::LoadAllExperimentRecordsResponse & response);

		QByteArray wrap(const Datastruct::ExperimentRecordDeleteRequest & request);
		bool unrap(const QByteArray & data, Datastruct::ExperimentRecordDeleteRequest & request);

		QByteArray wrap(const Datastruct::ExperimentRecordDeleteResponse & response);
		bool unrap(const QByteArray & data, Datastruct::ExperimentRecordDeleteResponse & response);

		QByteArray wrap(const Datastruct::ExperimentRecordModifyRequest & request);
		bool unrap(const QByteArray & data, Datastruct::ExperimentRecordModifyRequest & request);

		QByteArray wrap(const Datastruct::ExperimentRecordModifyResponse & response);
		bool unrap(const QByteArray & data, Datastruct::ExperimentRecordModifyResponse & response);

		// �й����ƽ̨
		QByteArray wrap(const Datastruct::DetectPlatformCreateRequest & request);
		bool unrap(const QByteArray & data, Datastruct::DetectPlatformCreateRequest & request);

		QByteArray wrap(const Datastruct::DetectPlatformCreateResponse & response);
		bool unrap(const QByteArray & data, Datastruct::DetectPlatformCreateResponse & response);

		QByteArray wrap(const Datastruct::LoadAllDetectPlatformsRequest & request);
		bool unrap(const QByteArray & data, Datastruct::LoadAllDetectPlatformsRequest & request);

		QByteArray wrap(const Datastruct::LoadAllDetectPlatformsResponse & response);
		bool unrap(const QByteArray & data, Datastruct::LoadAllDetectPlatformsResponse & response);

		QByteArray wrap(const Datastruct::DetectPlatformDeleteRequest & request);
		bool unrap(const QByteArray & data, Datastruct::DetectPlatformDeleteRequest & request);

		QByteArray wrap(const Datastruct::DetectPlatformDeleteResponse & response);
		bool unrap(const QByteArray & data, Datastruct::DetectPlatformDeleteResponse & response);

		QByteArray wrap(const Datastruct::DetectPlatformModifyRequest & request);
		bool unrap(const QByteArray & data, Datastruct::DetectPlatformModifyRequest & request);

		QByteArray wrap(const Datastruct::DetectPlatformModifyResponse & response);
		bool unrap(const QByteArray & data, Datastruct::DetectPlatformModifyResponse & response);

		// �й����ƽ̨����
		QByteArray wrap(const Datastruct::DetectPlatformSubtypeCreateRequest & request);
		bool unrap(const QByteArray & data, Datastruct::DetectPlatformSubtypeCreateRequest & request);

		QByteArray wrap(const Datastruct::DetectPlatformSubtypeCreateResponse & response);
		bool unrap(const QByteArray & data, Datastruct::DetectPlatformSubtypeCreateResponse & response);

		QByteArray wrap(const Datastruct::LoadAllDetectPlatformSubtypesRequest & request);
		bool unrap(const QByteArray & data, Datastruct::LoadAllDetectPlatformSubtypesRequest & request);

		QByteArray wrap(const Datastruct::LoadAllDetectPlatformSubtypesResponse & response);
		bool unrap(const QByteArray & data, Datastruct::LoadAllDetectPlatformSubtypesResponse & response);

		QByteArray wrap(const Datastruct::DetectPlatformSubtypeDeleteRequest & request);
		bool unrap(const QByteArray & data, Datastruct::DetectPlatformSubtypeDeleteRequest & request);

		QByteArray wrap(const Datastruct::DetectPlatformSubtypeDeleteResponse & response);
		bool unrap(const QByteArray & data, Datastruct::DetectPlatformSubtypeDeleteResponse & response);

		QByteArray wrap(const Datastruct::DetectPlatformSubtypeModifyRequest & request);
		bool unrap(const QByteArray & data, Datastruct::DetectPlatformSubtypeModifyRequest & request);

		QByteArray wrap(const Datastruct::DetectPlatformSubtypeModifyResponse & response);
		bool unrap(const QByteArray & data, Datastruct::DetectPlatformSubtypeModifyResponse & response);

		//Ŀ����Ϣ
		QByteArray wrap(const Datastruct::TargetCreateRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TargetCreateRequest & request);

		QByteArray wrap(const Datastruct::TargetCreateResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TargetCreateResponse & response);

		QByteArray wrap(const Datastruct::LoadAllTargetRequest & request);
		bool unrap(const QByteArray & data, Datastruct::LoadAllTargetRequest & request);

		QByteArray wrap(const Datastruct::LoadAllTargetResponse & response);
		bool unrap(const QByteArray & data, Datastruct::LoadAllTargetResponse & response);

		QByteArray wrap(const Datastruct::TargetDeleteRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TargetDeleteRequest & request);

		QByteArray wrap(const Datastruct::TargetDeleteResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TargetDeleteResponse & response);

		QByteArray wrap(const Datastruct::TargetModifyRequest & request);
		bool unrap(const QByteArray & data, Datastruct::TargetModifyRequest & request);

		QByteArray wrap(const Datastruct::TargetModifyResponse & response);
		bool unrap(const QByteArray & data, Datastruct::TargetModifyResponse & response);

		/** AIS���� **/
		QByteArray wrap(const Datastruct::AISDataCreateRequest & request);
		bool unrap(const QByteArray & data, Datastruct::AISDataCreateRequest & request);

		QByteArray wrap(const Datastruct::AISDataCreateResponse & response);
		bool unrap(const QByteArray & data, Datastruct::AISDataCreateResponse & response);

		QByteArray wrap(const Datastruct::LoadAllAISDataRequest & request);
		bool unrap(const QByteArray & data, Datastruct::LoadAllAISDataRequest & request);

		QByteArray wrap(const Datastruct::LoadAllAISDatasResponse & response);
		bool unrap(const QByteArray & data, Datastruct::LoadAllAISDatasResponse & response);

		QByteArray wrap(const Datastruct::AISDataByConditionRequest & request);
		bool unrap(const QByteArray & data, Datastruct::AISDataByConditionRequest & request);

		QByteArray wrap(const Datastruct::AISDataByConditionResponse & response);
		bool unrap(const QByteArray & data, Datastruct::AISDataByConditionResponse & response);

		QByteArray wrap(const Datastruct::AISDataDeleteRequest & request);
		bool unrap(const QByteArray & data, Datastruct::AISDataDeleteRequest & request);

		QByteArray wrap(const Datastruct::AISDataDeleteResponse & response);
		bool unrap(const QByteArray & data, Datastruct::AISDataDeleteResponse & response);

		QByteArray wrap(const Datastruct::AISDataModifyRequest & request);
		bool unrap(const QByteArray & data, Datastruct::AISDataModifyRequest & request);

		QByteArray wrap(const Datastruct::AISDataModifyResponse & response);
		bool unrap(const QByteArray & data, Datastruct::AISDataModifyResponse & response);



	private:
		JsonWrapper(QObject *parent = nullptr);

		/*! 
		 * @brief ��JSON��ԭʼ���ݽ�����������ûص�������
		 * @param data ����ԭʼ����
		 * @param callback �ص�������
		 * @return true:����ɹ�;false:���ʧ��
		 */
		bool unwrapObject(const QByteArray & data,std::function<void(QJsonObject &)> callback);

		QByteArray wrapObject(std::function<void(QJsonObject &)> callback);

	private:
		static JsonWrapper * m_instance;
		JsonKey m_jsonKey;
	};

} //namespace CommonDefines 