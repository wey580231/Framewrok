#include "taskoverviewitem.h"
#include "ui_taskoverviewitem.h"

#include <QRect>
#include <QHBoxLayout>

using namespace Base;

namespace Related {

#define CustomButtProp  "ButtProp"

	TaskOverViewItem::TaskOverViewItem(QWidget *parent)
		: QWidget(parent)
	{
		ui = new Ui::TaskOverViewItem();
		ui->setupUi(this);

		init();
	}

	TaskOverViewItem::~TaskOverViewItem()
	{
		delete ui;
	}

	void TaskOverViewItem::setTaskBaseInfo(Datastruct::TaskEntityData data)
	{
		m_taskData = data;

		ui->label_3->setText(data.taskName);
		ui->label_5->setText(data.startTime + QString("-") + data.endTime);
		ui->label_7->setText(data.location);
	}

	bool TaskOverViewItem::eventFilter(QObject *watched, QEvent *event)
	{
		if (watched == ui->taskOverItemContainer) {
			if (event->type() == QEvent::Enter) {
				m_toolButtWidget->setGeometry(QRect(QPoint(0, ui->taskOverItemContainer->height()), ui->taskOverItemContainer->size()));
				m_toolButtWidget->show();
				m_widgetAnimation.addAnimation(m_toolButtWidget, QRect(QPoint(0, 0), ui->taskOverItemContainer->size()));
			}
			else if (event->type() == QEvent::Leave) {
				m_widgetAnimation.addAnimation(m_toolButtWidget, QRect(QPoint(0, ui->taskOverItemContainer->height()), ui->taskOverItemContainer->size()), [&]() {
					m_toolButtWidget->hide();
				});
			}
		}
		return QWidget::eventFilter(watched, event);
	}

	void TaskOverViewItem::init()
	{
		m_toolButtWidget = new QWidget(ui->taskOverItemContainer);
		m_toolButtWidget->setStyleSheet("background-color:rgba(0,77,136, 210)");
		m_toolButtWidget->hide();

		m_openTaskButt = new Base::RIconButton();

		m_openTaskButt->disableColors(RIconButton::Color_All);
		m_openTaskButt->setToolTip(QStringLiteral("打开任务"));
		m_openTaskButt->setIconSize(Base::RIconButton::ICON_48);
		m_openTaskButt->setCheckedIcon(QIcon(QStringLiteral(":/QYBlue/resource/qyblue/打开_48_hover.png")));
		m_openTaskButt->setIcon(QIcon(QStringLiteral(":/QYBlue/resource/qyblue/打开_48.png")));
		m_openTaskButt->setProperty(CustomButtProp, Butt_Open);
		connect(m_openTaskButt, SIGNAL(clicked()),this,SLOT(respButtPressed()));

		m_deleteTaskButt = new Base::RIconButton();
		m_deleteTaskButt->disableColors(RIconButton::Color_All);
		m_deleteTaskButt->setToolTip(QStringLiteral("删除任务"));
		m_deleteTaskButt->setIconSize(Base::RIconButton::ICON_48);
		m_deleteTaskButt->setCheckedIcon(QIcon(QStringLiteral(":/QYBlue/resource/qyblue/删除_48.png")));
		m_deleteTaskButt->setIcon(QIcon(QStringLiteral(":/QYBlue/resource/qyblue/删除_48_hover.png")));
		m_deleteTaskButt->setProperty(CustomButtProp, Butt_Delete);
		connect(m_deleteTaskButt, SIGNAL(clicked()),this,SLOT(respButtPressed()));

		QGridLayout * glayout = new QGridLayout();
		glayout->setRowStretch(0, 1);
		glayout->setColumnStretch(0, 1);

		QHBoxLayout * hlayout = new QHBoxLayout();
		hlayout->addWidget(m_openTaskButt);
		hlayout->addSpacing(40);
		hlayout->addWidget(m_deleteTaskButt);

		glayout->addLayout(hlayout, 1, 1, 1, 1);
		glayout->setRowStretch(2, 1);
		glayout->setColumnStretch(2, 1);

		m_toolButtWidget->setLayout(glayout);

		ui->taskOverItemContainer->installEventFilter(this);
	}

	void TaskOverViewItem::respButtPressed()
	{
		ButtType btype = static_cast<ButtType>(QObject::sender()->property(CustomButtProp).toInt());
		if (btype == Butt_Open) {
			emit openTask(m_taskData.id);
		}
		else if (btype == Butt_Delete) {
			emit deleteTask(m_taskData.id);
		}
	}

} //namespace Related 