#include "operationtoolspage.h"

namespace Related {

	OperationToolsPage::OperationToolsPage(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	OperationToolsPage::~OperationToolsPage()
	{
	}

	void OperationToolsPage::init()
	{
		auto createButt = [](QString icon, QString text) {
			Base::RIconButton * butt = new Base::RIconButton();
			butt->setIconSize(Base::RIconButton::ICON_16);
			butt->setIcon(QIcon(icon));
			butt->setText(text);
			butt->setFont(QFont(QStringLiteral("΢���ź�"), 10));
			butt->setMinimumSize(60, 30);

			return butt;
		};

		m_addButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/����.png"), QStringLiteral("����"));
		m_delButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/ɾ��.png"), QStringLiteral("ɾ��"));
		m_editButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/�༭.png"), QStringLiteral("�༭"));
		m_refreshButton = createButt(QStringLiteral(":/QYBlue/resource/qyblue/ˢ��.png"), QStringLiteral("ˢ��"));

		QHBoxLayout * toolLayout = new QHBoxLayout();
		toolLayout->setContentsMargins(0, 0, 0, 0);
		toolLayout->addWidget(m_addButton);
		toolLayout->addWidget(m_delButton);
		toolLayout->addWidget(m_editButton);
		toolLayout->addWidget(m_refreshButton);
		toolLayout->addStretch(1);
		setLayout(toolLayout);
		setFixedHeight(40);
	}

}//namespace Related 