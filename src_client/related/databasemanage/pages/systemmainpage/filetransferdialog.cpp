#include "filetransferdialog.h"

#include <QDebug>

namespace Related {

	FileTransferDialog::FileTransferDialog(QWidget *parent)
		: Base::DialogProxy(parent)
	{
		setTitle(QStringLiteral("文件传输"));
		setMinimumSize(900, 650);

		m_fileUploadingItems.clear();
		m_fileUploadedItems.clear();

		init();
		initConnet();
	}

	FileTransferDialog::~FileTransferDialog()
	{
	}

	void FileTransferDialog::setFileData(QList<FileDescriptionData > list)
	{
		for (int i = 0; i < list.size(); i++)
		{
			FileDescriptionData description = list.at(i);

			FileUploadingItem * item = new FileUploadingItem();
			item->setFileDescriptionInfo(description);
			connect(item, SIGNAL(signalFileUploaded(FileUploadingItem *)), this, SLOT(slotFileUploaded(FileUploadingItem *)));

			m_fileUploadingItems.append(item);
		}
		// 刷新文件上传界面
		updateFileUploadingWidget();

		// 开始上传第一个文件
		if (m_fileUploadingItems.size() > 0) {
			FileUploadingItem * firstItem = m_fileUploadingItems.at(0);
			firstItem->setFileUploadType(FileUploadingItem::File_Uploading);
			firstItem->setFileUploading();
		}
	}

	void FileTransferDialog::slotFileUploaded(FileUploadingItem * item)
	{
		if (m_fileUploadingItems.size() > 0 ) {
			int  index = 0;
			for (int i = 0; i < m_fileUploadingItems.size() ; i++) {
				if (item == m_fileUploadingItems.at(i)) {
					index = i;
					break;
				}
			}
			m_fileUploadingItems.removeAt(index);
		}
		// 刷新文件上传界面
		updateFileUploadingWidget();
		item->setFileUploadType(FileUploadingItem::File_Uploaded);
		m_fileUploadedItems.append(item);
		// 刷新删除完毕界面
		updateFileUploadedWidget();
		// 上传下一个文件
		if (m_fileUploadingItems.size() > 0) {
			FileUploadingItem * nestItem = m_fileUploadingItems.at(0);
			nestItem->setFileUploadType(FileUploadingItem::File_Uploading);
			nestItem->setFileUploading();
		}
	}

	void FileTransferDialog::init()
	{
		QWidget * uploadingWidget = new QWidget();
		QWidget * transferCompleteWidget = new QWidget();

		m_tabWidget = new Base::RTabWidget();
		m_tabWidget->setTabBarHeight(40);
		m_tabWidget->setTabAligment(Base::AlignCenter);
		m_tabWidget->setSpacing(4);

		m_tabWidget->addPage(QStringLiteral("正在上传"), uploadingWidget);
		m_tabWidget->addPage(QStringLiteral("传输完成"), transferCompleteWidget);
		//正在上传
		{
			m_srollArea = new QScrollArea();
			m_srollArea->setStyleSheet("background-color:rgba(0,0,0,0)");
			m_srollArea->setWidgetResizable(true);

			m_widget = new QWidget();
			m_widget->setStyleSheet("background-color:rgba(0,0,0,0)");
			m_srollArea->setWidget(m_widget);

			//[] 主窗口
			QHBoxLayout * mainLayout = new QHBoxLayout();
			mainLayout->addWidget(m_srollArea);
			mainLayout->setContentsMargins(0, 0, 0, 0);
			uploadingWidget->setLayout(mainLayout);
		} 

		//传输完成
		{
			m_uploadedSrollArea = new QScrollArea();
			m_uploadedSrollArea->setStyleSheet("background-color:rgba(0,0,0,0)");
			m_uploadedSrollArea->setWidgetResizable(true);

			m_uploadedWidget = new QWidget();
			m_uploadedWidget->setStyleSheet("background-color:rgba(0,0,0,0)");
			m_uploadedSrollArea->setWidget(m_uploadedWidget);

			//[] 主窗口
			QHBoxLayout * layout = new QHBoxLayout();
			layout->addWidget(m_uploadedSrollArea);
			layout->setContentsMargins(0, 0, 0, 0);
			transferCompleteWidget->setLayout(layout);
		}

		QWidget * mainWidget = new QWidget();
		QHBoxLayout * mainLayout = new QHBoxLayout();
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(m_tabWidget);
		mainWidget->setLayout(mainLayout);

		this->setContentWidget(mainWidget);
	}

	void FileTransferDialog::initConnet()
	{


	}

	void FileTransferDialog::updateFileUploadingWidget()
	{
		QVBoxLayout * vlayout = nullptr;

		if (m_widget->layout() == nullptr) {
			vlayout = new QVBoxLayout();
			vlayout->setContentsMargins(0, 0, 0, 0);
			m_widget->setLayout(vlayout);
		}
		else {
			vlayout = dynamic_cast<QVBoxLayout *>(m_widget->layout());
			for (int i = vlayout->count() - 1; i >= 0; i--) {
				if (vlayout->itemAt(i)->widget()) {
					delete vlayout->takeAt(i);
				}
			}
		}

		for (int i = 0; i < m_fileUploadingItems.size(); i++) {
			vlayout->addWidget(m_fileUploadingItems.at(i));
			vlayout->setSpacing(0);
		}
		vlayout->addStretch();
	}

	void FileTransferDialog::updateFileUploadedWidget()
	{
		QVBoxLayout * vlayout = nullptr;

		if (m_uploadedWidget->layout() == nullptr) {
			vlayout = new QVBoxLayout();
			vlayout->setContentsMargins(0, 0, 0, 0);
			m_uploadedWidget->setLayout(vlayout);
		}
		else {
			vlayout = dynamic_cast<QVBoxLayout *>(m_uploadedWidget->layout());
			for (int i = vlayout->count() - 1; i >= 0; i--) {
				if (vlayout->itemAt(i)->widget()) {
					delete vlayout->takeAt(i);
				}
			}
		}

		for (int i = 0; i < m_fileUploadedItems.size(); i++) {
			vlayout->addWidget(m_fileUploadedItems.at(i));
			vlayout->setSpacing(0);
		}
	}

}//namespace Related 