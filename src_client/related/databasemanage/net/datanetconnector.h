/*!
 * @brief     数据链路连接   
 * @author    wey
 * @version   1.0
 * @date      2021.01.30 15:25:49
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

#include "netconnector.h"

namespace Related {

	class DataNetConnector : public NetConnector
	{
		Q_OBJECT

	public:
		static DataNetConnector * instance();
		~DataNetConnector();

		/*!
		 * @brief   用户有关
		 */
		void write(const Datastruct::UserLoginRequest & request);
		void write(const Datastruct::UserRegistRequest & request);
		void write(const Datastruct::LoadAllUserRequest & request);
		void write(const Datastruct::OperateUserRequest & request);

		/*!
		 * @brief 任务操作有关
		 */
		void write(const Datastruct::TaskCreateRequest & request);
		void write(const Datastruct::LoadAllTaskRequest & request);
		void write(const Datastruct::TaskByConditionRequest & request);
		void write(const Datastruct::TaskDeleteRequest & request);
		void write(const Datastruct::TaskSimpleRequest & request);
		void write(const Datastruct::TaskStaticsInfoRequest & request);
		void write(const Datastruct::TaskModifyRequest & request);
	
		/*!
		 * @brief 任务侦测平台
		 */
		void write(const Datastruct::TaskDetectPlatformCreateRequest & request);
		void write(const Datastruct::LoadAllTaskDetectPlatformRequest & request);
		void write(const Datastruct::TaskDetectPlatformByConditionRequest & request);
		void write(const Datastruct::TaskDetectPlatformDeleteRequest & request);
		void write(const Datastruct::TaskDetectPlatformModifyRequest & request);

		/*!
		 * @brief 任务试验图片资源
		 */
		void write(const Datastruct::TaskImageCreateRequest & request);
		void write(const Datastruct::LoadAllTaskImageRequest & request);
		void write(const Datastruct::TaskImageByConditionRequest & request);
		void write(const Datastruct::TaskImageDeleteRequest & request);
		void write(const Datastruct::TaskImageModifyRequest & request);
		
		/*!
		 * @brief  值班日志有关
		 */
		void write(const Datastruct::DutyRecordCreateRequest & request);
		void write(const Datastruct::LoadAllDutyRecordRequest & request);
		void write(const Datastruct::DutyRecordDeleteRequest & request);
		void write(const Datastruct::DutyRecordModifyRequest & request);

		/*!
		 * @brief   试验记录有关
		 */
		void write(const Datastruct::ExperimentRecordCreateRequest & request);
		void write(const Datastruct::LoadAllExperimentRecordsRequest & request);
		void write(const Datastruct::ExperimentRecordDeleteRequest & request);
		void write(const Datastruct::ExperimentRecordModifyRequest & request);

		/*!
		 * @brief   侦测平台有关
		 */
		void write(const Datastruct::DetectPlatformCreateRequest & request);
		void write(const Datastruct::LoadAllDetectPlatformsRequest & request);
		void write(const Datastruct::DetectPlatformDeleteRequest & request);
		void write(const Datastruct::DetectPlatformModifyRequest & request);

		/*!
		 * @brief   侦测平台亚型有关
		 */
		void write(const Datastruct::DetectPlatformSubtypeCreateRequest & request);
		void write(const Datastruct::LoadAllDetectPlatformSubtypesRequest & request);
		void write(const Datastruct::DetectPlatformSubtypeDeleteRequest & request);
		void write(const Datastruct::DetectPlatformSubtypeModifyRequest & request);

		/*!
		 * @brief   侦测平台亚型有关
		 */
		void write(const Datastruct::TargetCreateRequest & request);
		void write(const Datastruct::LoadAllTargetRequest & request);
		void write(const Datastruct::TargetDeleteRequest & request);
		void write(const Datastruct::TargetModifyRequest & request);

		/*!
		 * @brief   AIS数据有关
		 */
		void write(const Datastruct::AISDataCreateRequest & request);
		void write(const Datastruct::LoadAllAISDataRequest & request);
		void write(const Datastruct::AISDataDeleteRequest & request);
		void write(const Datastruct::AISDataModifyRequest & request);

	signals:
		void newClientData(QByteArray  array);

	private slots:
		void respRectNetData(QByteArray  array);

	protected:
		void processNetData(QByteArray & array);

	private:
		DataNetConnector();
		QByteArray makePacket(Datastruct::PacketType type, QByteArray & body);

	private:
		static DataNetConnector * m_instance;

	};

} //namespace Related 