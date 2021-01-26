#include "rlistwidget.h"

#include <qboxlayout.h>
#include <qpainter.h>
#include <qdebug.h>

#include <core\file\programfilepath.h>

namespace Related {

	RListWidgetItem::RListWidgetItem(QWidget * parent /*= nullptr*/):Base::RIconButton(parent), m_mouseEnter(false), m_expandModel(true), m_pageId(0)
	{
		m_colorCollect.m_normalTextColor = QColor(255, 255, 255,120);

		m_colorCollect.m_checkedTextColor = QColor(255,255,255);
		m_colorCollect.m_checkedBorderColor = QColor(216, 216, 216, 30);
		m_colorCollect.m_checkedBackGroundColor = QColor(0,220,246);
		
		setCheckable(true);
	}

	RListWidgetItem::RListWidgetItem()
	{

	}

	QSize RListWidgetItem::minimumSizeHint() const
	{
		return QSize(120,70);
	}

	void RListWidgetItem::setPageId(int pageId)
	{
		m_pageId = pageId;
	}

	void RListWidgetItem::paintEvent(QPaintEvent * e)
	{
		QPainter painter(this);

		QFont font;
		font.setFamily(QStringLiteral("微软雅黑"));
		if(m_expandModel)
			font.setPixelSize(19);
		else
			font.setPixelSize(12);
		font.setBold(true);
		painter.setFont(font);

		QMargins margins = contentsMargins();

		//底部线条
		QPoint leftBottom(margins.left(),height() - 1);
		QPoint rightBottom(width() - margins.right(),height() - 1);
		painter.setPen(m_colorCollect.m_checkedBorderColor);
		painter.drawLine(leftBottom, rightBottom);

		//绘制图标
		QSize fixIconSize(24,24);
		int paddingLeft = margins.left();

		if (m_expandModel)
			paddingLeft += 35;
		
		QPixmap pix = icon().pixmap(fixIconSize);
		int paddingTop = (height() - fixIconSize.height())/2;
		painter.drawPixmap(QPoint(paddingLeft,paddingTop), pix);

		//左侧选择指示线条
		if (m_mouseEnter || isChecked()) {
			painter.save();
			painter.setPen(Qt::NoPen);
			painter.setBrush(QBrush(m_colorCollect.m_checkedBackGroundColor));
			painter.drawRect(QRect(0,margins.top(),2,height() - margins.top() - margins.bottom()));
			painter.restore();
			
			painter.setPen(m_colorCollect.m_checkedTextColor);
		}
		else {
			painter.setPen(m_colorCollect.m_normalTextColor);
		}

		//右侧箭头
		QPoint polygons[3];
		polygons[0] = QPoint(width() - margins.right() - 5, height() / 2 - 5);
		polygons[1] = QPoint(width() - margins.right(), height() / 2);
		polygons[2] = QPoint(width() - margins.right() - 5, height() / 2 + 5);

		painter.drawLine(polygons[0], polygons[1]);
		painter.drawLine(polygons[1], polygons[2]);

		if (m_expandModel) {
			paddingLeft += 25;
			painter.drawText(QRect(paddingLeft + fixIconSize.width(),0,rect().width(),height()), Qt::AlignLeft | Qt::AlignVCenter, text());
		}
		else {
			QFontMetrics metrics(font);
			int textWidth = metrics.width(text());
			int offsetLeft = (paddingLeft + fixIconSize.width() / 2) - textWidth / 2;
			int offsetTop = paddingTop + pix.height();
			painter.drawText(QRect(offsetLeft,offsetTop,textWidth,height() - offsetTop),Qt::AlignCenter, text());
		}
	}

	void RListWidgetItem::enterEvent(QEvent * event)
	{
		m_mouseEnter = true;
		update();
	}

	void RListWidgetItem::leaveEvent(QEvent * event)
	{
		m_mouseEnter = false;
		update();
	}

	void RListWidgetItem::setExpandModel(bool flag)
	{
		m_expandModel = flag;

		if(m_expandModel)
			setContentsMargins(35, 25, 35, 25);
		else
			setContentsMargins(25, 25, 15, 25);

	}

	RListWidget::RListWidget(QWidget *parent)
		: QWidget(parent), m_expandModel(true)
	{
		init();
	}

	RListWidget::~RListWidget()
	{
	}

	void RListWidget::addItem(RListWidgetItem * item)
	{
		if (item == nullptr || m_items.contains(item))
			return;

		if (m_mainLayout->count() == 0) {
			m_mainLayout->addWidget(item);
			m_mainLayout->addStretch(1);
		}
		else {
			m_mainLayout->insertWidget(m_mainLayout->count() - 1, item);
		}

		m_buttGroup->addButton(item,item->pageId());

		m_items.append(item);
	}

	void RListWidget::addItem(int pageId,QString text, QIcon icon)
	{
		RListWidgetItem * item = new RListWidgetItem(this);
		item->setCheckable(true);
		item->setPageId(pageId);
		item->setText(text);
		item->setIcon(icon);
		addItem(item);
	}
	
	void RListWidget::setExpanded(bool enable)
	{
		m_expandModel = enable;
		
		std::for_each(m_items.begin(), m_items.end(), [&](RListWidgetItem * item) {
			item->setExpandModel(enable);
		});
	}

	void RListWidget::setCurrentIndex(int index)
	{
		if (index >= 0 && index < m_items.size())
			m_items.at(index)->setChecked(true);
	}

	int RListWidget::currentIndex() const
	{
		if (m_items.size() > 0)
			return m_items.first()->m_pageId;

		return -1;
	}

	void RListWidget::indexChanged(QAbstractButton * butt)
	{
		RListWidgetItem * item = dynamic_cast<RListWidgetItem *>(butt);

		if (m_items.contains(item))
			emit currentIndexChanged(item->m_pageId);
	}

	void RListWidget::init()
	{
		QWidget * mainWidget = new QWidget();

		m_buttGroup = new QButtonGroup();
		connect(m_buttGroup, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(indexChanged(QAbstractButton *)));

		m_mainLayout = new QVBoxLayout();
		m_mainLayout->setSpacing(0);
		m_mainLayout->setContentsMargins(0, 0, 0, 0);
		mainWidget->setLayout(m_mainLayout);

		QVBoxLayout * layout = new QVBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(mainWidget);
		setLayout(layout);
	}

} //namespace Related 