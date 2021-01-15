#include "util.h"

namespace Related {

	Util::Util(QObject *parent)
		: QObject(parent)
	{
	}

	Util::~Util()
	{
	}

	Base::RIconButton * Util::createButt(QString icon, QString text)
	{
		Base::RIconButton * butt = new Base::RIconButton();
		butt->setIconSize(Base::RIconButton::ICON_16);
		butt->setIcon(QIcon(icon));
		butt->setText(text);
		butt->setFont(QFont(QStringLiteral("Î¢ÈíÑÅºÚ"), 10));
		butt->setMinimumSize(60, 30);

		return butt;
	}

} //namespace Related 