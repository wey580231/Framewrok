/*!
 * @brief     ���ݿ�������Ϣ   
 * @author    wey
 * @version   1.0
 * @date      2021.01.23 10:51:04
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

namespace DB{

	/*!
	 *  @brief ֧�����ݿ�����
	 */
	enum DatabaseType
	{
		DB_NONE = 0,
		DB_MYSQL,               /*!< ʹ��MySql���ݿ�*/
		DB_ORACLE               /*!< ʹ��Oracle���ݿ�*/
	};

	/*!
	 *  @brief ���ݿ�������Ϣ
	 *  @note ���浱ǰʹ�õ����ݿ��еĹ����ص�
	 */
	struct DBFeature {
		bool lastInsertId;          /*!< �Ƿ�֧�ֲ�ѯ���������ݵ�id */
		bool transactions;          /*!< �Ƿ�֧������ */
	};

	/*!
	 *  @brief ���ݿ�������Ϣ
	 */
	struct DatabaseConfigInfo
	{
		DatabaseType dbType;		/*!< ���ݿ����� */
		QString hostName;			/*!< ������ */
		QString dbName;				/*!< ���������ݿ��� */
		QString dbUser;				/*!< �û��� */
		QString dbPass;				/*!< ���� */
		ushort port;				/*!< ���ݿ�˿� */
	};

};
