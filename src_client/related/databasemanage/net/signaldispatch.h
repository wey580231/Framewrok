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

	signals:
		void respUserLoginResponse(const Datastruct::UserLoginResponse & response);

	private:
		SignalDispatch(QObject *parent = nullptr);


	private:
		static SignalDispatch * m_instance;

		friend class JsonWrapper;
	};

} //namespace Related 