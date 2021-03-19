#include "fileuploadingitem.h"

#include <QDebug>

#include "../../customwidget/customwidgetcontainer.h"

namespace Related {

	FileUploadingItem::FileUploadingItem(QWidget *parent)
		: QWidget(parent), m_b_suspend(false), m_uploadingDataFile(nullptr)
	{
		this->setMinimumHeight(60);
		this->setMaximumHeight(65);

		init();
		initConnect();
	}

	FileUploadingItem::~FileUploadingItem()
	{

	}

	void FileUploadingItem::setFileUploadType(FileUploadType type)
	{
		m_fileUploadType = type;
		if (type == File_Uploading) {

		}
		else if (type == File_Uploaded)
		{
			m_uploadingButt->setVisible(false);
		}
	}

	void FileUploadingItem::setFileDescriptionInfo(FileDescriptionData  description)
	{
		m_description = description;
		//[] 设置文件描述
		m_uploadingDataFile->setFileDescription(description);
		//[] 刷新界面
		m_fileName->setText(description.m_name);
		QString size = QString("%1/%2").arg(Base::RUtil::switchBytesUnit(0)).arg(Base::RUtil::switchBytesUnit(description.m_size));
		m_fileSize->setText(size);
		m_progressBar->setValue(0); 
	}

	void FileUploadingItem::setFileUploading()
	{
		if (m_uploadingDataFile != NULL) {
			m_uploadingDataFile->startUploading();
		}
	}

	void FileUploadingItem::openFileBtn()
	{
		
	}

	void FileUploadingItem::slotSendFileStataus(bool status)
	{
		if (status) {
			emit signalFileUploaded(this);
		}
		else {
			emit signalFileUploaded(this);
		}
	}

	void FileUploadingItem::slotSendFileProgressStataus(qint64 totalLen, qint64 currentLen)
	{
		QString size = QString("%1/%2").arg(Base::RUtil::switchBytesUnit(currentLen)).arg(Base::RUtil::switchBytesUnit(totalLen));
		m_fileSize->setText(size);

		int index = (double)currentLen / (double)totalLen * 100;
		m_progressBar->setValue(index);
	}

	void FileUploadingItem::init()
	{
		//[1] 先实例化文件上传
		m_uploadingDataFile = new UploadingDataFile();
		connect(m_uploadingDataFile, SIGNAL(signalSendFileStataus(bool)), this, SLOT(slotSendFileStataus(bool)));
		connect(m_uploadingDataFile, SIGNAL(signalSendFileProgressStataus(qint64, qint64)), this, SLOT(slotSendFileProgressStataus(qint64, qint64)));

		CustomWidgetContainer * container = new CustomWidgetContainer();
		{
			m_fileName = new QLabel();

			m_fileSize = new QLabel();

			m_progressBar = new QProgressBar();
			m_progressBar->setRange(0, 100);
			
			m_uploadingButt = new Base::RIconButton();
			m_uploadingButt->setText(QStringLiteral("开始"));
			m_uploadingButt->setMinimumSize(60, 30);
			//m_uploadingButt->setIcon(QIcon(WRAP_RESOURCE(新增)));

			m_openFile = new Base::RIconButton();
			m_openFile->setMinimumSize(60, 30);
			m_openFile->setIcon(QIcon(QStringLiteral(":/QYBlue/resource/qyblue/打开_48_hover.png")));
			connect(m_openFile, SIGNAL(clicked()), this, SLOT(openFileBtn()));

			QWidget * widget = new QWidget();
			QHBoxLayout * layout = new QHBoxLayout();
			layout->addWidget(m_fileName, 2);
			layout->addWidget(m_fileSize, 2);
			layout->addStretch(1);
			layout->addWidget(m_progressBar, 2);
			layout->addStretch(1);
			layout->addWidget(m_uploadingButt,1);
			layout->addWidget(m_openFile);
			layout->setContentsMargins(0, 0, 0, 0);
			widget->setLayout(layout);
			container->setContent(widget);
		}

		//[] 主窗口布局
		QHBoxLayout * mainLayout = new QHBoxLayout();
		mainLayout->addWidget(container);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		this->setLayout(mainLayout);
	}

	void FileUploadingItem::initConnect()
	{

	}

}//namespace Related 