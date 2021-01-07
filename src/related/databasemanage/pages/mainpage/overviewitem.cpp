#include "overviewitem.h"

namespace Related {

	OverViewItem::OverViewItem(QWidget *parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
	}

	OverViewItem::~OverViewItem()
	{
	}

	void OverViewItem::setLabelText(QString text)
	{
		ui.labelItemText->setText(text);
	}

	void OverViewItem::setLabelData(QString text)
	{
		ui.labelItemData->setText(text);
	}

	void OverViewItem::setLabelIcon(QString icon)
	{
		ui.labelIcon->setStyleSheet(QString("border-image:url(%1)").arg(icon));
	}

	void OverViewItem::setLabelBackground(QColor color)
	{
		ui.rightContainer->setStyleSheet(QString("background-color:rgba(%1,%2,%3,255)").arg(color.red()).arg(color.green()).arg(color.blue()));
	}

} //namespace Related 