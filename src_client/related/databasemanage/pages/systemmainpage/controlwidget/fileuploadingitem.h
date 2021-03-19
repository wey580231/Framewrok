/*!
 * @brief     文件上传界面  
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
		 * @brief  文件上传类型
		 */
		enum FileUploadType {
			File_Uploading,				/*!< 正在上传 */
			File_Uploaded,				/*!< 上传完成 */
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
		QLabel * m_fileName;						/*!< 文件名 */
		QLabel * m_fileSize;						/*!< 文件大小 */
		QProgressBar * m_progressBar;				/*!< 上传进度 */	
		Base::RIconButton * m_uploadingButt;		/*!< 文件上传 */
		Base::RIconButton * m_openFile;				/*!< 打开文件 */

		FileUploadType m_fileUploadType;			/*!< 文件上传类型 */
		bool m_b_suspend;							/*!< 暂停 */

		FileDescriptionData  m_description;			/*!< 文件描述信息 */
		UploadingDataFile * m_uploadingDataFile;	/*!< 文件上传 */
	};
}//namespace Related 