#include "pageswitchbar.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QListView>
#include <QLineEdit>

#include "../utils/util.h"

#define BUTT_ID "CUSTOM_BUTT_ID"

namespace Related {

	PageSwitchBar::PageSwitchBar(QWidget *parent)
		: QWidget(parent), m_dataSize(0), m_pageCount(0), m_currPage(0), m_perPageItemCount(0)
	{
		setObjectName("mainWidget");
		setFixedHeight(35);

		init();
	}

	PageSwitchBar::~PageSwitchBar()
	{
	}

	void PageSwitchBar::setDataSize(int dataSize)
	{
		m_dataSize = dataSize;

		reCalc();
		updatePageIndicate();
	}

	void PageSwitchBar::respPageCountChanged(int pageIndex)
	{
		m_perPageItemCount = m_pageItemCountBox->currentData().toInt();
		emit perPageNumsChanged(m_perPageItemCount);
		reCalc();

		updatePageIndicate();
	}

	void PageSwitchBar::buttPressed()
	{
		ButtType btype = static_cast<ButtType>(QObject::sender()->property(BUTT_ID).toInt());

		switch (btype)
		{
		case FIRST_PAGE:
			m_currPage = 0;
			break;
		case PRI_PAGE:
			m_currPage -= 1;
			break;
		case NEXT_PAGE:
			m_currPage += 1;
			break;
		case LAST_PAGE:
			m_currPage = m_pageCount - 1;
			break;
		default:
			break;
		}

		if (m_currPage >= m_pageCount)
			m_currPage = m_pageCount - 1;

		if (m_currPage < 0)
			m_currPage = 0;

		updatePageIndicate();
	}

	void PageSwitchBar::init()
	{
		QHBoxLayout * pageLayout = new QHBoxLayout();
		pageLayout->setContentsMargins(4, 4, 4, 4);

		m_firstButton = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/到首页.png"), QStringLiteral("首页"));
		m_prePageButton = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/小于号.png"), QStringLiteral("上页"));
		m_nextPageButton = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/大于号.png"), QStringLiteral("下页"));
		m_lastButton = Util::createButt(QStringLiteral(":/QYBlue/resource/qyblue/到尾页.png"), QStringLiteral("尾页"));

		m_nextPageButton->setIconTextDirection(false);
		m_lastButton->setIconTextDirection(false);

		m_firstButton->setProperty(BUTT_ID, FIRST_PAGE);
		m_prePageButton->setProperty(BUTT_ID, PRI_PAGE);
		m_nextPageButton->setProperty(BUTT_ID, NEXT_PAGE);
		m_lastButton->setProperty(BUTT_ID, LAST_PAGE);

		connect(m_firstButton, SIGNAL(pressed()),this,SLOT(buttPressed()));
		connect(m_prePageButton, SIGNAL(pressed()),this,SLOT(buttPressed()));
		connect(m_nextPageButton, SIGNAL(pressed()),this,SLOT(buttPressed()));
		connect(m_lastButton, SIGNAL(pressed()),this,SLOT(buttPressed()));

		m_pageItemCountBox = new QComboBox();
		m_pageItemCountBox->setFocusPolicy(Qt::NoFocus);
		m_pageItemCountBox->setView(new QListView());
		m_pageItemCountBox->addItem(QStringLiteral("30条/页"), 30);
		m_pageItemCountBox->addItem(QStringLiteral("50条/页"), 50);
		m_pageItemCountBox->addItem(QStringLiteral("100条/页"), 100);

		m_perPageItemCount = m_pageItemCountBox->currentData().toInt();

		m_pageNumLabel = new QLineEdit();
		m_pageNumLabel->setReadOnly(true);
		m_pageNumLabel->setFixedWidth(60);
		m_pageNumLabel->setAlignment(Qt::AlignCenter);

		connect(m_pageItemCountBox, SIGNAL(currentIndexChanged(int)), this, SLOT(respPageCountChanged(int)));

		pageLayout->addStretch(1);
		pageLayout->addWidget(m_pageNumLabel);
		pageLayout->addWidget(m_pageItemCountBox);
		pageLayout->addWidget(m_firstButton);
		pageLayout->addWidget(m_prePageButton);
		pageLayout->addWidget(m_nextPageButton);
		pageLayout->addWidget(m_lastButton);
		pageLayout->addStretch(1);

		setLayout(pageLayout);
	}

	void PageSwitchBar::reCalc()
	{
		if(m_dataSize % m_perPageItemCount == 0)
			m_pageCount = m_dataSize / m_perPageItemCount;
		else
			m_pageCount = m_dataSize / m_perPageItemCount + 1;

		if (m_currPage >= m_pageCount)
			m_currPage = m_pageCount - 1;

		if (m_currPage < 0)
			m_currPage = 0;

		updateButtState();
	}

	void PageSwitchBar::updateButtState()
	{
		m_firstButton->setEnabled(m_currPage != 0);
		m_prePageButton->setEnabled(m_currPage > 0);
		m_nextPageButton->setEnabled(m_currPage < m_pageCount - 1);
		m_lastButton->setEnabled(m_currPage != (m_pageCount - 1));
	}

	void PageSwitchBar::updatePageIndicate()
	{
		m_pageNumLabel->setText(QString("%1 / %2").arg(m_currPage + 1).arg(m_pageCount));

		updateButtState();

		emit switchPage(m_currPage);
	}

} //namespace Related 