#include "newtaskdialog.h"

#include <QDebug>

namespace Related {

	NewTaskDialog::NewTaskDialog(QWidget *parent)
		: Base::DialogProxy(parent)
	{
		init();
		this->setMinimumSize(800, 600);
	}

	NewTaskDialog::~NewTaskDialog()
	{
	}

	void NewTaskDialog::init()
	{
		//[] 任务信息设置窗口
		m_newTaskInfoSetWidget = new NewTaskInfoSetWidget();

		// 原始数据文件管理窗口
		QWidget *originalFileManageWidget = new QWidget();
		{
			m_treeView = new Base::RTreeView();
			m_treeView->setFocusPolicy(Qt::FocusPolicy::NoFocus);
			m_treeView->setObjectName("mainWidget");
			connect(m_treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotTreeItemClicked(QModelIndex)));

			m_treeModel = new Base::RTreeModel();
			QStringList headList;
			headList << QStringLiteral("文件结构");
			m_treeModel->setHeaderData(headList);

			m_treeView->setModel(m_treeModel);

			//文件录入
			m_fileLineEdit = new QLineEdit();

			m_fileButt = new Base::RIconButton();
			m_fileButt->setText(QStringLiteral("选择文件"));
			m_fileButt->setMinimumSize(60, 30);
			connect(m_fileButt, SIGNAL(clicked()), this, SLOT(slotSeleteFile()));

			QWidget *filePathWidget = new QWidget();
			QHBoxLayout *hLayout = new QHBoxLayout();
			hLayout->addWidget(m_fileLineEdit);
			hLayout->addWidget(m_fileButt);
			filePathWidget->setLayout(hLayout);

			//
			QVBoxLayout *originalFileManageLayout = new QVBoxLayout();
			originalFileManageLayout->addWidget(m_treeView);
			originalFileManageLayout->addWidget(filePathWidget);
			originalFileManageLayout->setContentsMargins(0, 0, 0, 0);
			originalFileManageWidget->setLayout(originalFileManageLayout);
		}


		QWidget *mainWidget = new QWidget();
		QHBoxLayout * mainLayout = new QHBoxLayout();
		mainLayout->addWidget(m_newTaskInfoSetWidget);
		mainLayout->addWidget(originalFileManageWidget);
		mainLayout->setContentsMargins(4, 4, 4, 4);
		mainWidget->setLayout(mainLayout);
		this->setContentWidget(mainWidget);

		setButton(DialogProxy::Ok, this, SLOT(accept()));
		setButton(DialogProxy::Cancel, this, SLOT(reject()));
	}

	bool NewTaskDialog::FindFile(const QString & path)
	{
		QDir dir(path);
		if (!dir.exists()) {
			return false;
		}
		dir.setFilter(QDir::Dirs | QDir::Files);
		dir.setSorting(QDir::DirsFirst);
		QFileInfoList list = dir.entryInfoList();
		int i = 0;

		bool bIsDir;
		do {
			QFileInfo fileInfo = list.at(i);
			if (fileInfo.fileName() == "." | fileInfo.fileName() == "..") {
				++i;
				continue;
			}
			bIsDir = fileInfo.isDir();
			if (bIsDir) {
				
				Base::TreeNode *tempNode = createTreeNode(m_fileNode ,fileInfo.filePath());
				m_fileNode->nodes.append(tempNode);

				FindFile(fileInfo.filePath());
			}	
			else  {
				QString filePath = fileInfo.path();
				QFileInfo t_dirFileInfo(filePath);
				QString t_dirName = t_dirFileInfo.baseName();

				if (m_fileNode->nodes.size() > 0) {
					for (int i = 0; i < m_fileNode->nodes.size();i++) {
						Base::TreeNode * tempNode = m_fileNode->nodes.at(i);
						if (tempNode->nodeName == t_dirName) {
							Base::TreeNode *tempNode2 = createTreeNode(tempNode, fileInfo.filePath());
							tempNode->nodes.append(tempNode2);
						}
					}
				}
			}
			++i;
		} while (i < list.size());

		return true;
	}

	/*!
	 *  @brief 创建树节点
	 */
	Base::TreeNode * NewTaskDialog::createTreeNode(Base::TreeNode * parentNode, QString  pasth)
	{
		QFileInfo t_fileInfo(pasth);

		Base::TreeNode * node = new Base::TreeNode;
		node->nodeChecked = false;
		node->nodeName = t_fileInfo.baseName();
		node->parentNode = parentNode;
		return node;
	}

	void NewTaskDialog::updateModel()
	{
		m_treeModel->refreshModel();
		m_treeView->expandAll();
	}


	void NewTaskDialog::slotSeleteFile()
	{
		m_originalFilePath =  QFileDialog::getExistingDirectory(this, QStringLiteral("选择目录"),
			"./",
			QFileDialog::ShowDirsOnly
			| QFileDialog::DontResolveSymlinks);

		m_fileLineEdit->setText(m_originalFilePath);

		Base::TreeNode * parentNode = nullptr;
		m_fileNode = createTreeNode(parentNode, m_originalFilePath);

		FindFile(m_originalFilePath);

		m_treeModel->addRootNode(m_fileNode);

		qDebug() << "1512121212" << m_treeModel->rootNodeSize();
		this->updateModel();
	}

	void NewTaskDialog::slotTreeItemClicked(QModelIndex index)
	{
		qDebug() << "12121" << index.row() << index.column();

	}

	void NewTaskDialog::respOk()
	{
		respCancel();
	}

	void NewTaskDialog::respCancel() 
	{
		close();
	}

}//namespace Related 