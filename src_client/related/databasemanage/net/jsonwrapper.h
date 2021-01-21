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

#include <QObject>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "protocol.h"

namespace Related {

	class JsonWrapper : public QObject
	{
		Q_OBJECT

	public:
		static JsonWrapper * instance();
		~JsonWrapper();

		QByteArray wrap(PacketType type, const UserLoginRequest & request);

	private:
		JsonWrapper(QObject *parent = nullptr);

	private:
		static JsonWrapper * m_instance;

	};

} //namespace Related 