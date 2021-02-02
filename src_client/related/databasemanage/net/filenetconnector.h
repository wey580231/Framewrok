/*!
 * @brief     �ļ���������   
 * @author    wey
 * @version   1.0
 * @date      2021.02.02 08:18:12
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

#include "netconnector.h"

namespace Related {

	class FileNetConnector : public NetConnector
	{
		Q_OBJECT

	public:
		static FileNetConnector * instance();
		~FileNetConnector();

	protected:
		void processNetData(QByteArray & data);
		
	private:
		FileNetConnector();

	private:
		static FileNetConnector * m_instance;
	};

} //namespace Related 