#include "tablecolumneditwindow.h"

#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QDebug>

#include "../rmessagebox.h"

namespace Base {

	TableColumnEditWindow::TableColumnEditWindow(QWidget *parent)
		: DialogProxy(parent)
	{
		setButton(DialogProxy::Ok,this,SLOT(respOk()));
		setButton(DialogProxy::No,this,SLOT(respCancel()));
		init();

		setTitle(QStringLiteral("表格列调整"));
		setMinimumSize(350, 500);
	}

	TableColumnEditWindow::~TableColumnEditWindow()
	{
	}

	void TableColumnEditWindow::respOk()
	{
		m_newItems.clear();
		int visibleCount = 0;

		for (int i = 0; i < m_itemListWidget->count(); i++) {
			QListWidgetItem * item = m_itemListWidget->item(i);
			ColumnItem it;
			it.m_id = item->data(Qt::UserRole).toInt();
			it.m_name = item->text();
			it.m_visible = item->checkState() == Qt::Checked;
			it.m_align = static_cast<Qt::Alignment>(item->data(Qt::UserRole + 1).toInt());
			it.m_columnWidth = item->data(Qt::UserRole + 2).toInt();

			if (it.m_visible)
				visibleCount++;

			m_newItems << it;
		}

		if (visibleCount == 0) {
			RMessageBox::warning(this, QStringLiteral("提示"),QStringLiteral("至少勾选一列!"),RMessageBox::Yes);
			return;
		}

		accept();
	}

	void TableColumnEditWindow::respCancel()
	{
		reject();
	}

	void TableColumnEditWindow::changeItemIndex()
	{
		QString objName = QObject::sender()->objectName();

		QModelIndex curModelIndex = m_itemListWidget->currentIndex();

		if (!curModelIndex.isValid())
			return;

		int curRow = curModelIndex.row();

		if (objName == "UP") {
			if (curRow > 0) {
				QListWidgetItem * tmpItem = m_itemListWidget->takeItem(curRow);
				m_itemListWidget->insertItem(curRow - 1, tmpItem);
				m_itemListWidget->setCurrentRow(curRow - 1);
			}
		}
		else {
			if (curRow < m_itemListWidget->count() - 1) {
				QListWidgetItem * tmpItem = m_itemListWidget->takeItem(curRow);
				m_itemListWidget->insertItem(curRow + 1, tmpItem);
				m_itemListWidget->setCurrentRow(curRow + 1);
			}
		}
	}

	void TableColumnEditWindow::setItems(ColumnItems & items)
	{
		for (ColumnItem & item : items) {
			QListWidgetItem * it = new QListWidgetItem();
			it->setCheckState(item.m_visible ? Qt::Checked : Qt::Unchecked);
			it->setText(item.m_name);
			it->setData(Qt::UserRole, item.m_id);
			it->setData(Qt::UserRole + 1, (int)item.m_align);
			it->setData(Qt::UserRole + 2, item.m_columnWidth);

			m_itemListWidget->addItem(it);
		}
	}

	void TableColumnEditWindow::init()
	{
		QWidget * mainWidget = new QWidget();

		QVBoxLayout * vlayout = new QVBoxLayout();

		QSize fixSize(32,32);
		m_moveUp = new RIconButton();
		m_moveUp->setIcon(QIcon(QStringLiteral(":/QYBlue/resource/qyblue/向上.png")));

		m_moveDown = new RIconButton();
		m_moveDown->setIcon(QIcon(QStringLiteral(":/QYBlue/resource/qyblue/向下.png")));

		m_moveUp->setObjectName("UP");
		m_moveDown->setObjectName("DOWN");

		connect(m_moveUp, SIGNAL(pressed()), this, SLOT(changeItemIndex()));
		connect(m_moveDown, SIGNAL(pressed()), this, SLOT(changeItemIndex()));

		m_itemListWidget = new QListWidget();
		m_itemListWidget->setFocusPolicy(Qt::NoFocus);
		connect(m_itemListWidget, &QListWidget::currentRowChanged, [&](int row) {

			if (row == 0) {
				m_moveUp->setEnabled(false);
				m_moveDown->setEnabled(true);
			}
			else if (row == m_itemListWidget->count() - 1) {
				m_moveUp->setEnabled(true);
				m_moveDown->setEnabled(false);
			}
			else {
				m_moveUp->setEnabled(true);
				m_moveDown->setEnabled(true);
			}
		});

		m_moveUp->setFixedSize(fixSize);
		m_moveDown->setFixedSize(fixSize);

		vlayout->addStretch(1);
		vlayout->addWidget(m_moveUp);
		vlayout->addWidget(m_moveDown);
		vlayout->addStretch(1);

		QHBoxLayout * hlayout = new QHBoxLayout();
		hlayout->setContentsMargins(4, 4, 4, 4);
		hlayout->addWidget(m_itemListWidget);
		hlayout->addLayout(vlayout);

		mainWidget->setLayout(hlayout);

		setContentWidget(mainWidget);
	}

} //namespace Base 