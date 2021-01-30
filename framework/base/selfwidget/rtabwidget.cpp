#include "rtabwidget.h"

#include <QVBoxLayout>

namespace Base {

	RTabWidget::RTabWidget(QWidget *parent)
		: QWidget(parent)
	{
		init();
	}

	RTabWidget::~RTabWidget()
	{

	}

	void RTabWidget::addPage(QString tabName, QWidget * widget)
	{
		if (tabName.isEmpty() || widget == nullptr)
			return;

		int index = m_tabBar->tabCount();

		Base::RTabButton * newButt = new Base::RTabButton(tabName);
		m_tabBar->addTabButton(newButt, index);

		m_widgetContainer->addWidget(widget);
	}

	int RTabWidget::pageCount() const
	{
		return m_widgetContainer->count();
	}

	QWidget * RTabWidget::currentWidget()
	{
		if (m_widgetContainer->count() > 0)
			return m_widgetContainer->currentWidget();

		return nullptr;
	}

	void RTabWidget::setTabBarHeight(int fixedHeight)
	{
		m_tabBar->setFixedHeight(fixedHeight);
	}

	int RTabWidget::tabBarHeight() const
	{
		return m_tabBar->height();
	}

	void RTabWidget::setSpacing(int spacing)
	{
		if (layout())
			layout()->setSpacing(spacing);
	}

	void RTabWidget::setContentMargins(int left, int top, int right, int bottom)
	{
		if (layout())
			layout()->setContentsMargins(left, top, right, bottom);
	}

	void RTabWidget::setTabAligment(TabAlignment alignment)
	{
		m_tabBar->setTabAlignment(alignment);
	}

	void RTabWidget::setWidgetBringToBottomCallback(WidgetCallback callback)
	{
		m_bingToBottomCallback = callback;
	}

	void RTabWidget::setWidgetBringToTopCallback(WidgetCallback callback)
	{
		m_bringToTopCallback = callback;
	}

	void  RTabWidget::setSwitchWidget(QWidget * widget)
	{
		int t_index = 0;
		int t_count = m_widgetContainer->count();
		for (int i = 0; i < t_count; i ++) {
			QWidget * t_tempWidget = m_widgetContainer->widget(i);
			if (t_tempWidget == widget) {
				t_index = i;
			}
		}
		m_tabBar->setCurrentIndex(t_index);
		prepareSwitchWidget(t_index);
	}

	void RTabWidget::prepareSwitchWidget(int index)
	{
		QWidget * preWidget = m_widgetContainer->currentWidget();

		if (m_bingToBottomCallback)
			m_bingToBottomCallback(preWidget);

		m_widgetContainer->setCurrentIndex(index);

		if (m_bringToTopCallback)
			m_bringToTopCallback(m_widgetContainer->currentWidget());
	}

	void RTabWidget::init()
	{
		m_tabBar = new RTabBar();
		m_widgetContainer = new QStackedWidget();

		connect(m_tabBar, SIGNAL(currentIndexChanged(int)), this, SLOT(prepareSwitchWidget(int)));

		QVBoxLayout * vlayout = new QVBoxLayout();
		vlayout->setContentsMargins(0, 0, 0, 0);
		vlayout->addWidget(m_tabBar);
		vlayout->addWidget(m_widgetContainer);

		setLayout(vlayout);
	}

} //namespace Base