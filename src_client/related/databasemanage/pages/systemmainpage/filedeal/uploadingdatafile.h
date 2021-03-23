/*!
 * @brief     �����ļ�
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.03.15 16:31:25
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QObject>
#include <QThread>
#include <QFileInfo>
#include <QFile>

#include <base/util/rutil.h>
#include <commondefines/protocol.h>

#include "../../datastruct.h"

#define  SEND_FILE_LENGTH  63*1024		

namespace Related {

	class UploadingDataFile : public QObject
	{
		Q_OBJECT

	public:
		UploadingDataFile(QObject *parent = nullptr);
		~UploadingDataFile();

		void setFileDescription(FileDescriptionData description);

		void startUploading();

		void pauseSendFile();
		void continueSendFile();

	signals:
		void signalSendFileProgressStataus(qint64 totalLen, qint64 currentLen);
		void signalSendFileStataus(bool status);

	private slots:
		void processTaskDataFileCeateResponse(const Datastruct::TaskDataFileCreateResponse & response);
		void slotSendFileStatus(Datastruct::ConnectionType type);

	private:
		void initConnect();
		void sendCreateTaskDataFileInfo(Datastruct::TaskDataFileCreateRequest request);

		bool openFile();
		void dealFile();
		void closeFile();
	
	private:
		FileDescriptionData m_fileDescription;				/*!< �ļ����� */
		QString m_fileId;									/*!< �ļ�Id */
		qint64 m_currentLength;								/*!< ��ʱ���ͳ��� */
		QFile * m_file;										/*!< �ļ����ָ�� */
		bool m_b_sendFile;									/*!< �����ļ� */
		bool m_pause;					
	};

}//namespace Related 