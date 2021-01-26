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
		butt->setFont(QFont(QStringLiteral("微软雅黑"), 10));
		butt->setMinimumSize(60, 30);

		return butt;
	}

	Base::RMessageBox::StandardButton Util::showQuestion(QWidget * parent, QString questionContent)
	{
		return Base::RMessageBox::information(parent, QStringLiteral("提示"), questionContent,Base::RMessageBox::Yes | Base::RMessageBox::No, Base::RMessageBox::Yes);
	}

	Base::RMessageBox::StandardButton Util::showInformation(QWidget * parent, QString infoContent)
	{
		return Base::RMessageBox::information(parent, QStringLiteral("提示"), infoContent,Base::RMessageBox::Yes, Base::RMessageBox::Yes);
	}

	Base::RMessageBox::StandardButton Util::showWarning(QWidget * parent, QString warnContent)
	{
		return Base::RMessageBox::warning(parent, QStringLiteral("警告"),warnContent,Base::RMessageBox::Yes, Base::RMessageBox::Yes);
	}

} //namespace Related 