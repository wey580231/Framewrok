#include "gatherwidget.h"

#include <QDebug>

gatherwidget::gatherwidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->init();
}

gatherwidget::~gatherwidget()
{
}

void gatherwidget::slotSignalOut()
{
	emit signalSignalOut();
}

void gatherwidget::init()
{
	connect(ui.pushButton_20, SIGNAL(clicked()), this, SLOT(slotSignalOut()));

}
