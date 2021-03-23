/*!
 * @brief     �ļ��ϴ�����  
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.03.18 09:51:22
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>

#include <base/selfwidget/iconbutton.h>
#include <base/util/rutil.h>

#include "../../datastruct.h"
#include "../filedeal/uploadingdatafile.h"

namespace Related {

	class FileUploadingItem : public QWidget
	{
		Q_OBJECT

	public:
		FileUploadingItem(QWidget *parent = Q_NULLPTR);
		~FileUploadingItem();
		
		/*!
		 * @brief  �ļ��ϴ�����
		 */
		enum FileUploadType {
			File_Uploading,				/*!< �����ϴ� */
			File_Uploaded,				/*!< �ϴ���� */
		};

		void setFileUploadType(FileUploadType type);

		void setFileDescriptionInfo(FileDescriptionData description);
		void setFileUploading();
	
	signals:
		void signalFileUploaded(FileUploadingItem * item);

	private slots:
		void openFileBtn();
		void slotSendFileStataus(bool status);
		void slotSendFileProgressStataus(qint64 totalLen, qint64 currentLen);

	private:
		void init();
		void initConnect();

	private:
		QString m_itemId;							/*!< Id */
		QLabel * m_fileName;						/*!< �ļ��� */
		QLabel * m_fileSize;						/*!< �ļ���С */
		QProgressBar * m_progressBar;				/*!< �ϴ����� */	
		Base::RIconButton * m_uploadingButt;		/*!< �ļ��ϴ� */
		Base::RIconButton * m_openFile;				/*!< ���ļ� */

		FileUploadType m_fileUploadType;			/*!< �ļ��ϴ����� */
		bool m_b_suspend;							/*!< ��ͣ */

		FileDescriptionData  m_description;			/*!< �ļ�������Ϣ */
		UploadingDataFile * m_uploadingDataFile;	/*!< �ļ��ϴ� */
	};
}//namespace Related 