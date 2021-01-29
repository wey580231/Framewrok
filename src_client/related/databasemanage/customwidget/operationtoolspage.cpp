#include "operationtoolspage.h"

#include <QVariant>
#include <QDebug>

#include "../utils/util.h"
#include "../global.h"

namespace Related {

#define ButtId "ButtId"

	OperationToolsPage::OperationToolsPage(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	OperationToolsPage::~OperationToolsPage()
	{
	}

	void OperationToolsPage::setButtVisible(ButtTypes butts, bool visible /*= false*/)
	{
		uint tmp = 1;
		while (tmp <= Butt_Refresh) {
			uint sb = tmp & butts;
			if (sb) {
				ButtType bt = static_cast<ButtType>(tmp);	
				if (m_buttMap.contains(bt))
					m_buttMap[bt]->setVisible(visible);
			}

			tmp <<= 1;
		}
	}

	void OperationToolsPage::respButtCliecked()
	{
		emit buttPressed(static_cast<ButtType>(QObject::sender()->property(ButtId).toInt()));
	}

	void OperationToolsPage::init()
	{
		m_addButton = Util::createButt(WRAP_RESOURCE(ÐÂÔö), QStringLiteral("ÐÂÔö"));
		m_delButton = Util::createButt(WRAP_RESOURCE(É¾³ý), QStringLiteral("É¾³ý"));
		m_editButton = Util::createButt(WRAP_RESOURCE(±à¼­), QStringLiteral("±à¼­"));
		m_refreshButton = Util::createButt(WRAP_RESOURCE(Ë¢ÐÂ), QStringLiteral("Ë¢ÐÂ"));

		m_buttMap.insert(Butt_Add, m_addButton);
		m_buttMap.insert(Butt_Delete, m_delButton);
		m_buttMap.insert(Butt_Edit, m_editButton);
		m_buttMap.insert(Butt_Refresh, m_refreshButton);

		m_addButton->setProperty(ButtId, Butt_Add);
		m_delButton->setProperty(ButtId, Butt_Delete);
		m_editButton->setProperty(ButtId, Butt_Edit);
		m_refreshButton->setProperty(ButtId, Butt_Refresh);

		connect(m_addButton, SIGNAL(pressed()), this, SLOT(respButtCliecked()));
		connect(m_delButton, SIGNAL(pressed()), this, SLOT(respButtCliecked()));
		connect(m_editButton, SIGNAL(pressed()), this, SLOT(respButtCliecked()));
		connect(m_refreshButton, SIGNAL(pressed()), this, SLOT(respButtCliecked()));

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