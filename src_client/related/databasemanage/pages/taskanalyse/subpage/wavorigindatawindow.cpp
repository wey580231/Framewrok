#include "wavorigindatawindow.h"
#include "ui_wavorigindatawindow.h"

#include <QHBoxLayout>

namespace Related {

	WavOriginDataWindow::WavOriginDataWindow(QWidget *parent)
		: Base::DialogProxy(parent)
	{
		m_ui = new Ui::WavOriginDataWindow();

		init();

		this->setFixedSize(420,350);
		setTitle(QStringLiteral("原始采集数据"));

		setButton(Base::DialogProxy::Yes, this, SLOT(close()));
	}

	WavOriginDataWindow::~WavOriginDataWindow()
	{
	}

	void WavOriginDataWindow::init()
	{
		QWidget * mainWidget = new QWidget();
		m_ui->setupUi(mainWidget);
		setContentWidget(mainWidget);
	}

} //namespace Related 