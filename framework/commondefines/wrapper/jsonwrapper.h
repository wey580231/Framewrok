/*!
 * @brief     将数据转换成JSON格式或解析网络数据包   
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

		QByteArray wrap(const Datastruct::TaskCreateRequest & response);
		bool unrap(const QByteArray & data, Datastruct::TaskCreateRequest & response);

		//TODO 20210125 有关任务



		//TODO 20210125 有关值班日志
		QByteArray wrap(const Datastruct::DutyRecordCreateRequest & request);
		bool unrap(const QByteArray & data, Datastruct::DutyRecordCreateRequest & request);

		QByteArray wrap(const Datastruct::DutyRecordCreateResponse & response);
		bool unrap(const QByteArray & data, Datastruct::DutyRecordCreateResponse & response);

		QByteArray wrap(const Datastruct::LoadAllDutyRecordRequest & request);
		bool unrap(const QByteArray & data, Datastruct::LoadAllDutyRecordRequest & request);

		QByteArray wrap(const Datastruct::LoadAllDutyRecordResponse & response);
		bool unrap(const QByteArray & data, Datastruct::LoadAllDutyRecordResponse & response);

		QByteArray wrap(const Datastruct::ExperimentRecordDeleteRequest & request);
		bool unrap(const QByteArray & data, Datastruct::ExperimentRecordDeleteRequest & request);

		QByteArray wrap(const Datastruct::ExperimentRecordDeleteResponse & response);
		bool unrap(const QByteArray & data, Datastruct::ExperimentRecordDeleteResponse & response);

		//TODO 20210125 23:15:29 有关试验记录


		QByteArray wrap(const Datastruct::OperateUserRequest & request);
		bool unrap(const QByteArray & data, Datastruct::OperateUserRequest & request);

		QByteArray wrap(const Datastruct::OperateUserResponse & response);
		bool unrap(const QByteArray & data, Datastruct::OperateUserResponse & response);


	private:
		JsonWrapper(QObject *parent = nullptr);

		/*! 
		 * @brief 用JSON对原始数据解包，解包后调用回调处理函数
		 * @param data 网络原始数据
		 * @param callback 回调处理函数
		 * @return true:解包成功;false:解包失败
		 */
		bool unwrapObject(const QByteArray & data,std::function<void(QJsonObject &)> callback);

		QByteArray wrapObject(std::function<void(QJsonObject &)> callback);

	private:
		static JsonWrapper * m_instance;
		JsonKey m_jsonKey;
	};

} //namespace CommonDefines 