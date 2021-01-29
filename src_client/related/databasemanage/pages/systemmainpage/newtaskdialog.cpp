#include "newtaskdialog.h"

#include <QDebug>
#include <QTableWidget>
#include <QDir>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>

#include "../../customwidget/customwidgetcontainer.h"
#include "../../net/netconnector.h"
#include "../../net/signaldispatch.h"
#include "../../utils/util.h"
#include "../../global.h"

namespace Related {

	NewTaskDialog::NewTaskDialog(QWidget *parent)
		: Base::DialogProxy(parent)
	{
		init();
		initConnect();

		setTitle(QStringLiteral("新建任务"));
		setMinimumSize(900, 650);

		setButton(DialogProxy::Ok, this, SLOT(respOk()));
		setButton(DialogProxy::Cancel, this, SLOT(reject()));
	}

	NewTaskDialog::~NewTaskDialog()
	{

	}

	void NewTaskDialog::init()
	{
		m_newTaskWidget = new NewTaskInfoSetWidget();

		QWidget * imageWidget = new QWidget();
		QWidget * fileWidget = new QWidget();

		m_tabWidget = new Base::RTabWidget();
		m_tabWidget->setTabBarHeight(40);
		m_tabWidget->setTabAligment(Base::AlignCenter);
		m_tabWidget->setSpacing(4);

		m_tabWidget->addPage(QStringLiteral("基本信息"), m_newTaskWidget);
		m_tabWidget->addPage(QStringLiteral("任务图片"), imageWidget);
		m_tabWidget->addPage(QStringLiteral("任务数据"), fileWidget);

		//图片页面
		{
			Base::RIconButton * chooseImagePath = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/上传_32_32.png"), QStringLiteral("选择图片"));
			Base::RIconButton * clearImage = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/删除.png"), QStringLiteral("清空"));
			connect(chooseImagePath, SIGNAL(clicked()),this,SLOT(openLocalImage()));
			connect(clearImage, SIGNAL(clicked()),this,SLOT(clearImage()));

			QWidget * toolWidget = new QWidget();
			QHBoxLayout * toolLayout = new QHBoxLayout();
			toolLayout->setContentsMargins(0, 0, 0, 0);
			toolLayout->addWidget(chooseImagePath);
			toolLayout->addWidget(clearImage);
			toolLayout->addStretch(1);
			toolWidget->setLayout(toolLayout);

			m_tableView = new Base::RTableView();
			m_tableView->setFocusPolicy(Qt::NoFocus);
			m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

			m_imageTableModel = new ImageModel();
			m_tableView->setModel(m_imageTableModel);

			m_tableView->addColumnItem(Base::ColumnItem(Img_Id, QStringLiteral("索引"),70));
			m_tableView->addColumnItem(Base::ColumnItem(Img_Name, QStringLiteral("文件名"),300));
			m_tableView->addColumnItem(Base::ColumnItem(Img_FileTimeStamp, QStringLiteral("修改时间"), 180));
			m_tableView->addColumnItem(Base::ColumnItem(Img_FileType, QStringLiteral("类型"), 80));
			m_tableView->addColumnItem(Base::ColumnItem(Img_FileSize, QStringLiteral("大小"), 80));

			QVBoxLayout * vlayout = new QVBoxLayout();
			vlayout->setContentsMargins(4, 4, 4, 4);
			vlayout->addWidget(toolWidget);
			vlayout->addWidget(m_tableView);
			imageWidget->setLayout(vlayout);
		}

		{
			Base::RIconButton * chooseDir = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/上传_32_32.png"), QStringLiteral("选择目录"));
			Base::RIconButton * clearImage = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/删除.png"), QStringLiteral("清空"));
			connect(chooseDir, SIGNAL(clicked()), this, SLOT(slotSeleteFile()));
			connect(clearImage, SIGNAL(clicked()), this, SLOT(clearImage()));

			m_dataFilePath = new QLineEdit();
			m_dataFilePath->setReadOnly(true);
			m_dataFilePath->setMinimumWidth(120);

			QWidget * toolWidget = new QWidget();
			QHBoxLayout * toolLayout = new QHBoxLayout();
			toolLayout->setContentsMargins(0, 0, 0, 0);
			toolLayout->addWidget(chooseDir);
			toolLayout->addWidget(clearImage);
			toolLayout->addStretch(1);
			toolLayout->addWidget(m_dataFilePath);
			toolWidget->setLayout(toolLayout);

			m_treeView = new QTreeView();
			m_treeView->setObjectName("mainWidget");
			m_treeView->setFocusPolicy(Qt::FocusPolicy::NoFocus);

			m_treeModel = new QFileSystemModel();
			m_treeView->setModel(m_treeModel);

			QVBoxLayout * vlayout = new QVBoxLayout();
			vlayout->addWidget(toolWidget);
			vlayout->addWidget(m_treeView);
			vlayout->setContentsMargins(4, 4, 4, 4);
			fileWidget->setLayout(vlayout);
		}

		QWidget * mainWidget = new QWidget();
		QHBoxLayout * mainLayout = new QHBoxLayout();
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(m_tabWidget);
		mainWidget->setLayout(mainLayout);

		this->setContentWidget(mainWidget);
	}

	void NewTaskDialog::initConnect()
	{
		connect(SignalDispatch::instance(), SIGNAL(respTaskCreateResponse(const Datastruct::TaskCreateResponse &)),
			this, SLOT(processTaskCreateResponse(const Datastruct::TaskCreateResponse &)));
	}

	void NewTaskDialog::slotSeleteFile()
	{
		QString filePath = QFileDialog::getExistingDirectory(this, QStringLiteral("选择目录"),"./", QFileDialog::DontResolveSymlinks);
		if (filePath.isEmpty())
			return;

		m_originalFilePath = filePath;
		m_dataFilePath->setText(m_originalFilePath);

		m_treeModel->setRootPath(filePath);
	}

	void NewTaskDialog::respOk()
	{
		m_taskBaseInfo = m_newTaskWidget->getTaskBaseInfo();
		sendTaskBaseInfo();
	}

	void NewTaskDialog::openLocalImage()
	{
		QStringList imageList = QFileDialog::getOpenFileNames(this, QStringLiteral("选择任务图片"),"/home", "Images (*.png *.jpg)");
		if (imageList.isEmpty())
			return;

		QList<QFileInfo> fileInfos;
		for (QString fileName : imageList) {
			QFileInfo fileInfo(fileName);
			fileInfos.append(fileInfo);
		}

		m_imageTableModel->updateData(fileInfos);
	}

	void NewTaskDialog::clearImage()
	{
		Base::RMessageBox::StandardButton result = Util::showQuestion(this, QStringLiteral("是否清空数据表?"));
		if (result == Base::RMessageBox::Yes) {
			m_imageTableModel->clearData();
		}
	}

	void NewTaskDialog::processTaskCreateResponse(const Datastruct::TaskCreateResponse & response)
	{
		if(response.m_createResult){
			this->accept();
		}
		else
		{
			int result = Base::RMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("创建任务失败，请从新填写信息。"), Base::RMessageBox::Yes | Base::RMessageBox::No);
			if (result != Base::RMessageBox::Yes) {
				this->reject();
			}
		}
	}

	void NewTaskDialog::sendTaskBaseInfo()
	{
		Datastruct::TaskCreateRequest request;
		request.taskId = Base::RUtil::UUID();
		request.taskName = m_taskBaseInfo.taskName;
		request.startTime = m_taskBaseInfo.startTime;
		request.endTime = m_taskBaseInfo.endTime;
		request.location = m_taskBaseInfo.taskLocation;
		request.lon = m_taskBaseInfo.lon;
		request.lat = m_taskBaseInfo.lat;
		request.description = QStringLiteral("description");

		NetConnector::instance()->write(request);
	}

}//namespace Related 