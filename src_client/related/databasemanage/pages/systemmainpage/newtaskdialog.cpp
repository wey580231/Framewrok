#include "newtaskdialog.h"

#include <QDebug>
#include <QTableWidget>
#include <QDir>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>

#include "../../customwidget/customwidgetcontainer.h"
#include "../../net/datanetconnector.h"
#include "../../net/signaldispatch.h"
#include "../../utils/util.h"
#include "../../global.h"

#include "rawfiledeal/taskrawdatafilecheckthread.h"
#include "rawfiledeal/taskrawdatafiledealthread.h"

namespace Related {

	NewTaskDialog::NewTaskDialog(QString taskId, TaskEditType type, QWidget *parent)
		: Base::DialogProxy(parent), 
		m_taskId(taskId),
		m_taskEditType(type)
	{
		switch (type) {
		case Related::NewTaskDialog::Task_New: {
			setTitle(QStringLiteral("新建任务"));
		}
			break;
		case Related::NewTaskDialog::Task_Modify: {
			setTitle(QStringLiteral("修改任务"));
		}
			break;
		default:
			break;
		}
		setMinimumSize(900, 650);

		init();
		initConnect();
		setButton(DialogProxy::Ok, this, SLOT(respOk()));
		setButton(DialogProxy::Cancel, this, SLOT(reject()));
	}

	NewTaskDialog::~NewTaskDialog()
	{

	}

	void NewTaskDialog::setTaskBaseInfo(TaskBaseInfo info)
	{
		m_newTaskWidget->setTaskBaseInfo(info);
	}

	void NewTaskDialog::setTaskImages(QList<Datastruct::TaskImageEntityData> list)
	{
		m_imageTableModel->updateData(list);
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
			Base::RIconButton * chooseImagePath = Util::createButt(WRAP_RESOURCE(上传_32_32), QStringLiteral("选择图片"));
			chooseImagePath->setObjectName("image_choose");

			Base::RIconButton * clearImage = Util::createButt(WRAP_RESOURCE(删除), QStringLiteral("清空"));
			clearImage->setObjectName("image_clear");

			connect(chooseImagePath, SIGNAL(clicked()), this, SLOT(openLocalFile()));
			connect(clearImage, SIGNAL(clicked()), this, SLOT(respClearFile()));
			
			m_viewModelSwitch = Util::createButt(WRAP_RESOURCE(缩略图), QStringLiteral(""));
			m_viewModelSwitch->disableColors(Base::RIconButton::Color_BackGround | Base::RIconButton::Color_Borer);
			m_viewModelSwitch->setCheckedIcon(QIcon(WRAP_RESOURCE(列表)));
			m_viewModelSwitch->setCheckable(true);
			m_viewModelSwitch->setFixedWidth(32);
			connect(m_viewModelSwitch, SIGNAL(clicked(bool)),this,SLOT(switchViewModel(bool)));
			
			QWidget * toolWidget = new QWidget();
			QHBoxLayout * toolLayout = new QHBoxLayout();
			toolLayout->setContentsMargins(0, 0, 0, 0);
			toolLayout->addWidget(chooseImagePath);
			toolLayout->addWidget(clearImage);
			toolLayout->addStretch(1);
			toolLayout->addWidget(m_viewModelSwitch);
			toolWidget->setLayout(toolLayout);

			m_imageStack = new QStackedWidget();

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

			m_cardModel = new QScrollArea();
			m_cardModel->setObjectName("mainWidget");
			m_cardModel->setWidgetResizable(true);

			m_imageStack->addWidget(m_tableView);
			m_imageStack->addWidget(m_cardModel);

			QVBoxLayout * vlayout = new QVBoxLayout();
			vlayout->setContentsMargins(4, 4, 4, 4);
			vlayout->addWidget(toolWidget);
			vlayout->addWidget(m_imageStack);
			imageWidget->setLayout(vlayout);
		}

		{
			Base::RIconButton * chooseDir = Util::createButt(WRAP_RESOURCE(上传_32_32), QStringLiteral("选择目录"));
			chooseDir->setObjectName("file_choose");

			Base::RIconButton * clearImage = Util::createButt(WRAP_RESOURCE(删除), QStringLiteral("清空"));
			clearImage->setObjectName("file_clear");

			connect(chooseDir, SIGNAL(clicked()), this, SLOT(openLocalFile()));
			connect(clearImage, SIGNAL(clicked()), this, SLOT(respClearFile()));

			m_dataFilePath = new QLineEdit();
			m_dataFilePath->setReadOnly(true);
			m_dataFilePath->setMinimumWidth(320);

			QWidget * toolWidget = new QWidget();
			QHBoxLayout * toolLayout = new QHBoxLayout();
			toolLayout->setContentsMargins(0, 0, 0, 0);
			toolLayout->addWidget(chooseDir);
			toolLayout->addWidget(clearImage);
			toolLayout->addStretch(1);
			toolLayout->addWidget(m_dataFilePath);
			toolWidget->setLayout(toolLayout);

			m_fileTableView = new Base::RTableView();
			m_fileTableView->setFocusPolicy(Qt::NoFocus);
			m_fileTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
			m_fileTableView->setSelectionMode(QAbstractItemView::SingleSelection);

			m_fileTableModel = new ImageModel();
			m_fileTableView->setModel(m_fileTableModel);

			m_fileTableView->addColumnItem(Base::ColumnItem(Img_Id, QStringLiteral("索引"), 70));
			m_fileTableView->addColumnItem(Base::ColumnItem(Img_Name, QStringLiteral("文件名"), 300));
			m_fileTableView->addColumnItem(Base::ColumnItem(Img_FileTimeStamp, QStringLiteral("修改时间"), 180));
			m_fileTableView->addColumnItem(Base::ColumnItem(Img_FileType, QStringLiteral("类型"), 80));
			m_fileTableView->addColumnItem(Base::ColumnItem(Img_FileSize, QStringLiteral("大小"), 80));

			QVBoxLayout * vlayout = new QVBoxLayout();
			vlayout->addWidget(toolWidget);
			vlayout->addWidget(m_fileTableView);
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

		connect(SignalDispatch::instance(), SIGNAL(respTaskModifyResponse(const Datastruct::TaskModifyResponse &)),
			this, SLOT(processTaskModifyResponse(const Datastruct::TaskModifyResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respTaskDetectPlatformCreateResponse(const Datastruct::TaskDetectPlatformCreateResponse &)),
			this, SLOT(processTaskDetectPlatformCreateResponse(const Datastruct::TaskDetectPlatformCreateResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respTaskDetectPlatformModifyResponse(const Datastruct::TaskDetectPlatformModifyResponse &)),
			this, SLOT(processTaskDetectPlatformModifyResponse(const Datastruct::TaskDetectPlatformModifyResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respTaskImageCreateResponse(const Datastruct::TaskImageCreateResponse &)),
			this, SLOT(processTaskImageCeateResponse(const Datastruct::TaskImageCreateResponse &)));

		connect(SignalDispatch::instance(), SIGNAL(respTaskImageModifyResponse(const Datastruct::TaskImageModifyResponse &)),
			this, SLOT(processTaskImageModifyResponse(const Datastruct::TaskImageModifyResponse &)));
	}

	void NewTaskDialog::respOk()
	{
		switch (m_taskEditType)
		{
		case Related::NewTaskDialog::Task_New: {
			//[] 任务基本信息 
			m_taskBaseInfo = m_newTaskWidget->getTaskBaseInfo();
			sendNewTaskBaseInfo();

			sendNewTaskDetectPlatformInfo();

			//[] 图片基本信息
			if (m_imageTableModel->datasSize()> 0) {
				QList<Datastruct::TaskImageEntityData> listInfos = m_imageTableModel->getdatas();
				for (int i = 0; i < listInfos.size(); i++) {
					Datastruct::TaskImageEntityData info = listInfos.at(i);
					sendNewTaskImageInfo(info);
				}
			}
			//[] 原始数据文件
			if (m_rawDataList.size() > 0)  {
				TaskRawDataFileDealThread * dealThread = new TaskRawDataFileDealThread();
				dealThread->setTaskId(m_taskId);
				dealThread->setFilePats(m_rawDataList);
				dealThread->startMe();
			}
		}
			break;
		case Related::NewTaskDialog::Task_Modify: {
			m_taskBaseInfo = m_newTaskWidget->getTaskBaseInfo();
			sendModifyTaskBaseInfo();
		}
			break;
		default:
			break;
		}

	}

	void NewTaskDialog::openLocalFile()
	{
		bool fileChoose = QObject::sender()->objectName().contains("file");

		QStringList imageList;

		if (fileChoose) {
			//imageList = QFileDialog::getOpenFileNames(this, QStringLiteral("选择原始文件"), "/home", "Files (*.dat)");

			QString filePath = QFileDialog::getExistingDirectory(this, QStringLiteral("请选择路径..."), "/ home");

			TaskRawDataFileCheckThread * t_taskRawDataFileCheck = new TaskRawDataFileCheckThread();
			t_taskRawDataFileCheck->setRawDataFileRootPath(filePath);
			t_taskRawDataFileCheck->checkRawDataDir();
			imageList = t_taskRawDataFileCheck->getFileLists();
			m_rawDataList = imageList;
		}
		else {
			imageList = QFileDialog::getOpenFileNames(this, QStringLiteral("选择任务图片"),"/home", "Images (*.png *.jpg)");
		}
		if (imageList.isEmpty())
			return;

		QList<Datastruct::TaskImageEntityData> fileInfos;
		for (QString fileName : imageList) {
			QFileInfo fileInfo(fileName);
			Datastruct::TaskImageEntityData data;
			data.realName = fileInfo.baseName();	
			data.suffix = fileInfo.suffix();
			data.uploadTime = fileInfo.created().toString("yyyy-MM-dd hh:mm:ss");
			data.imageSize = Base::RUtil::switchBytesUnit(fileInfo.size()).toDouble();
			fileInfos.append(data);
		}

		if (fileChoose) {
			m_fileTableModel->updateData(fileInfos);
		}
		else {
			m_imageTableModel->updateData(fileInfos);
		}
	}

	void NewTaskDialog::respClearFile()
	{
		Base::RMessageBox::StandardButton result = Util::showQuestion(this, QStringLiteral("是否清空数据表?"));
		if (result == Base::RMessageBox::Yes) {
			if (QObject::sender()->objectName().contains("image")) {
				m_imageTableModel->clearData();
			}
			else {
				m_fileTableModel->clearData();
			}
		}
	}

	void NewTaskDialog::switchViewModel(bool isChecked)
	{
		if (isChecked) {
			m_viewModelSwitch->setToolTip(QStringLiteral("切换至列表模式"));
			m_imageStack->setCurrentWidget(m_cardModel);
		}
		else {
			m_viewModelSwitch->setToolTip(QStringLiteral("切换至缩略图模式"));
			m_imageStack->setCurrentWidget(m_tableView);
		}
	}

	void NewTaskDialog::processTaskCreateResponse(const Datastruct::TaskCreateResponse & response)
	{
		if(response.m_createResult){
			this->accept();
		}
		else
		{
			int result = Base::RMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("创建任务失败，请从新填写信息。"), Base::RMessageBox::Yes);
			if (result != Base::RMessageBox::Yes) {
				this->reject();
			}
		}
	}

	void NewTaskDialog::processTaskModifyResponse(const Datastruct::TaskModifyResponse & response)
	{
		if (response.m_result) {
			this->accept();
		}
		else
		{
			int result = Base::RMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("创建任务失败，请从新填写信息。"), Base::RMessageBox::Yes);
			if (result != Base::RMessageBox::Yes) {
				this->reject();
			}
		}
	}

	void NewTaskDialog::processTaskDetectPlatformCreateResponse(const Datastruct::TaskDetectPlatformCreateResponse & response)
	{
		qDebug() << "__________________processTaskDetectPlatformCreateResponse___________________";
	}

	void NewTaskDialog::processTaskDetectPlatformModifyResponse(const Datastruct::TaskDetectPlatformModifyResponse & response)
	{
	}

	void NewTaskDialog::processTaskImageCeateResponse(const Datastruct::TaskImageCreateResponse & response)
	{
		if (response.m_createResult) {
			qDebug() << "_____________processTaskImageCeateResponse_____________";
		}
		else
		{
		}
	}

	void NewTaskDialog::processTaskImageModifyResponse(const Datastruct::TaskImageModifyResponse & response)
	{
	}

	void NewTaskDialog::sendNewTaskBaseInfo()
	{
		Datastruct::TaskCreateRequest request;
		request.m_taskId = m_taskId;
		request.m_taskName = m_taskBaseInfo.taskName;
		request.m_startTime = m_taskBaseInfo.startTime;
		request.m_endTime = m_taskBaseInfo.endTime;
		request.m_location = m_taskBaseInfo.taskLocation;
		request.lon = m_taskBaseInfo.lon;
		request.lat = m_taskBaseInfo.lat;
		request.description = QStringLiteral("description");

		DataNetConnector::instance()->write(request);
	}

	void NewTaskDialog::sendModifyTaskBaseInfo()
	{
		Datastruct::TaskModifyRequest request;
		request.taskId = m_taskId;
		request.taskName = m_taskBaseInfo.taskName;
		request.startTime = m_taskBaseInfo.startTime;
		request.endTime = m_taskBaseInfo.endTime;
		request.location = m_taskBaseInfo.taskLocation;
		request.lon = m_taskBaseInfo.lon;
		request.lat = m_taskBaseInfo.lat;
		request.description = QStringLiteral("description");

		DataNetConnector::instance()->write(request);
	}

	void NewTaskDialog::sendNewTaskDetectPlatformInfo()
	{
		Datastruct::TaskDetectPlatformCreateRequest request;
		request.m_id = Base::RUtil::UUID();
		request.m_taskId = m_taskId;
		request.m_detectId = 1;
		request.m_name = QStringLiteral("测试平台");
		request.m_sensorType = QStringLiteral("测试类型");
		request.m_platformPower = 5;
		request.m_startTime = QStringLiteral("2021-01-31 22:56:55");
		request.m_endTime = QStringLiteral("2021-01-31 22:56:55");
		request.m_lastTime = 10;	
		DataNetConnector::instance()->write(request);
	}

	void NewTaskDialog::sendModifyTaskDetectPlatformInfo()
	{
	}

	void NewTaskDialog::sendNewTaskImageInfo(Datastruct::TaskImageEntityData info)
	{
		Datastruct::TaskImageCreateRequest request;
		request.m_id = Base::RUtil::UUID();						
		request.m_taskId = m_taskId;					
		request.m_realName = info.realName;
		request.m_suffix = info.suffix;
		request.m_uploadTime = info.uploadTime;
		request.m_imageSize = info.imageSize;
		request.m_description = QStringLiteral("description");
		DataNetConnector::instance()->write(request);
	}

	void NewTaskDialog::sendModifyTaskImageInfo()
	{
	}

}//namespace Related 