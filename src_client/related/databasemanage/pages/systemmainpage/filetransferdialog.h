/*!
 * @brief     文件传输对话框
 * @details	  用于显示文件上传文件记录，正在上传的文件列表
 * @author    yzg
 * @version   1.0
 * @date      2021.03.16 11:06:33
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMap>

#include <base/selfwidget/dialogproxy.h>
#include <base\selfwidget\tablemode\rtableview.h>
#include <base/selfwidget/iconbutton.h>
#include <base/selfwidget/rtabwidget.h>
#include <base\selfwidget\rmessagebox.h>
#include <base/util/rutil.h>

#include "../../datastruct.h"
#include "fileuploadingitem.h"
#include "filedeal/uploadingdatafile.h"

namespace Related {

	class FileTransferDialog : public Base::DialogProxy
	{
		Q_OBJECT

	public:
		FileTransferDialog(QWidget *parent);
		~FileTransferDialog();

		void setFileData(QList<FileDescriptionData > list);
	
	private slots:
		void slotFileUploaded(FileUploadingItem * item);

	private:
		void init();
		void initConnet();
		void updateFileUploadingWidget();
		void updateFileUploadedWidget();

	private:
		Base::RTabWidget * m_tabWidget;					

		QScrollArea * m_srollArea;
		QWidget * m_widget;

		QScrollArea * m_uploadedSrollArea;
		QWidget * m_uploadedWidget;

		QList<FileUploadingItem *> m_fileUploadingItems;						/*!< 正在上传列表 */
		QList<FileUploadingItem *> m_fileUploadedItems;							/*!< 上传完成列表 */
	};

}//namespace Related 