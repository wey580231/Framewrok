#include "loadingdialog.h"

#include <QHBoxLayout>
#include <QEvent>
#include <QPainter>
#include <QTimer>

#include <base/util/rutil.h>
#include <core/widgets/mainwindow.h>

namespace Related {

	LoadingDialog::LoadingDialog(QWidget *parent)
		: Base::DialogProxy(parent), m_imageIndex(0), m_accending(true)
	{
		init();
		setTitle(QStringLiteral("数据处理中..."));
		this->setFixedSize(240, 220);
		this->setWindowFlag(Qt::WindowStaysOnTopHint, true);

		for (int i = 1; i < 12; i++) {
			QPixmap pix(QString(QStringLiteral(":/QYBlue/resource/qyblue/loading/%1.png").arg(i)));
			m_imageList.append(pix.scaled(QSize(60, 60)));
		}

		m_switchTimer = new QTimer();
		m_switchTimer->setInterval(25);
		connect(m_switchTimer, SIGNAL(timeout()), this, SLOT(switchImage()));
	}

	LoadingDialog::~LoadingDialog()
	{

	}

	void LoadingDialog::showMe(QWidget * parent)
	{
		if (parent == nullptr) {
			this->setParent(Core::MainWindow::instance());
		}
		else {
			this->setParent(parent);
		}

		move(Base::RUtil::placeAInBCenter(size(),parentWidget()->size()));

		m_switchTimer->start();

		show();
	}

	void LoadingDialog::hideMe()
	{
		this->setParent(nullptr);
		m_switchTimer->stop();

		QTimer::singleShot(200, this, [&]() {
			hide();
		});
	}

	bool LoadingDialog::eventFilter(QObject * obj, QEvent * event)
	{
		if (obj == m_mainWidget) {
			if (event->type() == QEvent::Paint) {
				QPainter painter(m_mainWidget);

				QPixmap & pix = m_imageList[m_imageIndex];
				QSize windowSize = m_mainWidget->size();
				QSize psize = pix.size();
				painter.drawPixmap(QPoint((windowSize.width() - psize.width()) / 2, (windowSize.height() - psize.height()) / 2), pix);
			}
		}
		return Base::DialogProxy::eventFilter(obj, event);
	}

	void LoadingDialog::switchImage()
	{
		if (m_accending) {
			++m_imageIndex;

			if (m_imageIndex >= m_imageList.size()) {
				m_accending = false;
				m_imageIndex = m_imageList.size() - 1;
			}
		}
		else {
			--m_imageIndex;

			if (m_imageIndex < 0) {
				m_accending = true;
				m_imageIndex = 0;
			}
		}

		update();
	}

	void LoadingDialog::init()
	{
		m_mainWidget = new QWidget();
		m_mainWidget->installEventFilter(this);

		this->setContentWidget(m_mainWidget);
	}

} //namespace Related 