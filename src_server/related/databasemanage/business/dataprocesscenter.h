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

	private:
		Base::Database * m_database;

	};

} //