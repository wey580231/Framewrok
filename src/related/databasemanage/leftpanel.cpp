#include "leftpanel.h"

#include <qboxlayout.h>
#include <qdebug.h>

#include "customwidget\rlistwidget.h"

namespace Related {

	LeftPanel::LeftPanel(QWidget *parent)
		: QWidget(parent), m_expandStateWidth(276), m_contractionWidth(100)
	{
		init();
		respLeftPanelExpand(true);
		setMaximumWidth(m_expandStateWidth);
	}

	LeftPanel::~LeftPanel()
	{
	}

	void LeftPanel::respLeftPanelExpand(bool checked)
	{
		m_listWidget->setExpanded(checked);

		if (checked) {
			m_prgoramIcon->setText(QStringLiteral("学院"));
			m_expandButt->setIcon(QIcon(QStringLiteral(":/QYBlue/resource/qyblue/导航收缩.png")));
			m_expandButt->setToolTip(QStringLiteral("折叠"));
		}
		else {
			m_prgoramIcon->setText(QStringLiteral("QT"));
			m_expandButt->setIcon(QIcon(QStringLiteral(":/QYBlue/resource/qyblue/导航展开.png")));
			m_expandButt->setToolTip(QStringLiteral("展开"));
		}
	}

	void LeftPanel::init()
	{
		QWidget * mainWidget = new QWidget();
		mainWidget->setStyleSheet("background-color:rgba(43,67,128,255)");

		m_prgoramIcon = new QLabel(mainWidget);
		m_prgoramIcon->setObjectName("Label_ProgramName");

		m_expandButt = new Base::IconButton(mainWidget);
		m_expandButt->setCheckable(true);
		m_expandButt->setChecked(true);
		m_expandButt->setIconSize(Base::IconButton::ICON_24);
		m_expandButt->setContentsMargins(0,0,0,0);
		m_expandButt->setFixedSize(32, 32);

		m_expandButt->disableColor(Base::IconButton::Color_NormalBorder);
		m_expandButt->disableColor(Base::IconButton::Color_HoverBorder);
		m_expandButt->disableColor(Base::IconButton::Color_CheckedBorder);
		m_expandButt->disableColor(Base::IconButton::Color_CheckedBackGround);

		connect(m_expandButt, &Base::IconButton::clicked, [&](bool clicked) {
			if (clicked) {
				m_animation.addVariantAnimation(m_contractionWidth, m_expandStateWidth,[&](QVariant val) {
				this->setFixedWidth(val.toInt());
				});
			}
			else {
				m_animation.addVariantAnimation(m_expandStateWidth, m_contractionWidth, [&](QVariant val) {
					this->setFixedWidth(val.toInt());
				});
			}

			respLeftPanelExpand(clicked);
		});

		QHBoxLayout * tmpLayout = new QHBoxLayout();
		tmpLayout->addWidget(m_prgoramIcon);
		tmpLayout->addWidget(m_expandButt);
		
		QWidget * tmpWidget = new QWidget();
		tmpWidget->setLayout(tmpLayout);
		tmpWidget->setFixedHeight(100);

		m_listWidget = new RListWidget(this);
		m_listWidget->addItem(QStringLiteral("主页"), QIcon(QStringLiteral(":/QYBlue/resource/qyblue/首页icon.png")));
		m_listWidget->addItem(QStringLiteral("数据管理"), QIcon(QStringLiteral(":/QYBlue/resource/qyblue/数据管理icon.png")));
		m_listWidget->addItem(QStringLiteral("数据分析"), QIcon(QStringLiteral(":/QYBlue/resource/qyblue/数据分析icon.png")));
		m_listWidget->addItem(QStringLiteral("系统设置"), QIcon(QStringLiteral(":/QYBlue/resource/qyblue/系统设置icon.png")));

		m_listWidget->setCurrentIndex(0);

		QVBoxLayout * mainLayout = new QVBoxLayout();
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(tmpWidget);
		mainLayout->addWidget(m_listWidget);
		mainWidget->setLayout(mainLayout);

		QVBoxLayout * layout = new QVBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(mainWidget);
		setLayout(layout);
	}

} //namespace Related 