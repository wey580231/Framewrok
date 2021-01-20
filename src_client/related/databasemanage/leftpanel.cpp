#include "leftpanel.h"

#include <qboxlayout.h>
#include <qdebug.h>

#include "customwidget\rlistwidget.h"
#include "datastruct.h"

namespace Related {

	LeftPanel::LeftPanel(QWidget *parent)
		: QWidget(parent), m_expandStateWidth(276), m_contractionWidth(100)
	{
		init();
		respLeftPanelExpand(true);
		setFixedWidth(m_expandStateWidth);
	}

	LeftPanel::~LeftPanel()
	{
	}

	void LeftPanel::respLeftPanelExpand(bool checked)
	{
		m_listWidget->setExpanded(checked);

		m_notifyButt->setTextVisible(checked);

		if (checked) {
			m_prgoramIcon->setText(QStringLiteral("数据管理系统软件"));
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
		mainWidget->setObjectName("Widget_LeftPanel");

		m_prgoramIcon = new QLabel(mainWidget);
		m_prgoramIcon->setObjectName("Label_ProgramName");

		m_expandButt = new Base::RIconButton(mainWidget);
		m_expandButt->setCheckable(true);
		m_expandButt->setChecked(true);
		m_expandButt->setIconSize(Base::RIconButton::ICON_24);
		m_expandButt->setContentsMargins(0,0,0,0);
		m_expandButt->setFixedSize(32, 32);

		m_expandButt->disableColor(Base::RIconButton::Color_NormalBorder);
		m_expandButt->disableColor(Base::RIconButton::Color_HoverBorder);
		m_expandButt->disableColor(Base::RIconButton::Color_CheckedBorder);
		m_expandButt->disableColor(Base::RIconButton::Color_CheckedBackGround);

		connect(m_expandButt, &Base::RIconButton::clicked, [&](bool clicked) {
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
		connect(m_listWidget, SIGNAL(currentIndexChanged(int)),this,SIGNAL(currentIndexChanged(int)));

		m_listWidget->addItem(Page_MainPage,QStringLiteral("任务统计"), QIcon(QStringLiteral(":/QYBlue/resource/qyblue/首页icon.png")));
		m_listWidget->addItem(Page_DataManage,QStringLiteral("数据管理"), QIcon(QStringLiteral(":/QYBlue/resource/qyblue/数据管理icon.png")));
		m_listWidget->addItem(Page_DataAnalyse,QStringLiteral("数据分析"), QIcon(QStringLiteral(":/QYBlue/resource/qyblue/数据分析icon.png")));
		m_listWidget->addItem(Page_Setting,QStringLiteral("系统设置"), QIcon(QStringLiteral(":/QYBlue/resource/qyblue/系统设置icon.png")));

		m_listWidget->setCurrentIndex(0);

		QSize iconSize(40, 40);
		QFont iconFont(QStringLiteral("微软雅黑"),11);
		Base::RIconButton::ColorChooses disableColors = Base::RIconButton::Color_NormalBorder | Base::RIconButton::Color_CheckedBorder
			| Base::RIconButton::Color_CheckedBorder | Base::RIconButton::Color_HoverBackground | Base::RIconButton::Color_HoverBorder;

		auto setButtonProop = [&](Base::RIconButton * butt,QString text,QString pixmap) {
			butt->setIconTextSpacing(15);
			butt->setTextFont(iconFont);
			butt->setIconSize(Base::RIconButton::ICON_24);
			butt->disableColors(disableColors);
			butt->enableColor(Base::RIconButton::Color_NormalText, QColor(255, 255, 255, 120));
			butt->enableColor(Base::RIconButton::Color_HoverText, QColor(255, 255, 255));
			butt->setText(text);
			butt->setIcon(QIcon(pixmap));
		};

		m_notifyButt = new Base::RIconButton();
		setButtonProop(m_notifyButt, QStringLiteral("通知"), QStringLiteral(":/QYBlue/resource/qyblue/通知.png"));

		QWidget * bottomWidget = new QWidget();
		QVBoxLayout * bottomLayout = new QVBoxLayout();
		bottomLayout->setSpacing(15);
		bottomLayout->setContentsMargins(9, 9, 9, 15);
		bottomLayout->addWidget(m_notifyButt);
		bottomWidget->setLayout(bottomLayout);
		bottomWidget->setFixedHeight(iconSize.height() * bottomLayout->count() + (bottomLayout->count() - 1) * bottomLayout->spacing());

		QHBoxLayout * bottomHorizonalLayout = new QHBoxLayout();
		bottomHorizonalLayout->addStretch(1);
		bottomHorizonalLayout->addWidget(bottomWidget);
		bottomHorizonalLayout->addStretch(1);

		QVBoxLayout * mainLayout = new QVBoxLayout();
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(tmpWidget);
		mainLayout->addWidget(m_listWidget);
		mainLayout->addLayout(bottomHorizonalLayout);
		mainWidget->setLayout(mainLayout);

		QVBoxLayout * layout = new QVBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(mainWidget);
		setLayout(layout);
	}

} //namespace Related 