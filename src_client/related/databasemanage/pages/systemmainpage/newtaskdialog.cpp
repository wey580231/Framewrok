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
	}

	void NewTaskDialog::respOk()
	{
		m_taskBaseInfo = m_newTaskWidget->getTaskBaseInfo();
		sendTaskBaseInfo();
	}

	void NewTaskDialog::openLocalFile()
	{
		bool fileChoose = QObject::sender()->objectName().contains("file");

		QStringList imageList;
		if (fileChoose) {
			imageList = QFileDialog::getOpenFileNames(this, QStringLiteral("选择原始文件"), "/home", "Files (*.data)");
		}
		else {
			imageList = QFileDialog::getOpenFileNames(this, QStringLiteral("选择任务图片"),"/home", "Images (*.png *.jpg)");
		}
		if (imageList.isEmpty())
			return;

		QList<QFileInfo> fileInfos;
		for (QString fileName : imageList) {
			QFileInfo fileInfo(fileName);
			fileInfos.append(fileInfo);
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

		DataNetConnector::instance()->write(request);
	}

}//namespace Related 