#include "rmessagebox.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>
#include <QCoreApplication>
#include "qmath.h"

#include "../util/rsingleton.h"
#include "../util/rutil.h"
#include "../util/imagemanager.h"
#include "iconbutton.h"
#include "rlabel.h"
#include "toolbar.h"

namespace Base {

#define WINDOW_MARGIN_SIZE 5             //边距宽度
#define WINDOW_MARGIN_WIDTH 10           //渐变背景宽度
#define WINDOW_CURSOR_DETECT_SIZE  5     //鼠标移动在边框移动时检测的范围,最好WINDOW_MARGIN_SIZE与此值一致
#define ABSTRACT_TOOL_BAR_HEGIHT 30      //工具栏固定高度

	class RMessageBoxPrivate
	{
		Q_DECLARE_PUBLIC(RMessageBox)
	private:
		RMessageBoxPrivate(RMessageBox * q) :q_ptr(q)
		{
			initWidget();
			clickButton = RMessageBox::Cancel;
		}

		~RMessageBoxPrivate()
		{
			foreach(RIconButton * tmp, buttList)
			{
				delete tmp;
			}
			buttList.clear();
		}

		RMessageBox * q_ptr;

		void initWidget();
		void setIcon(RMessageBox::Icon type);
		void updateButtLayout();

		RIconLabel * iconLabel;
		QLabel * contentLabel;

		ToolBar * toolBar;
		QWidget * bottomWidget;

		QPoint  mousePressPoint;

		RMessageBox::Icon mesType;

		QList<RIconButton *> buttList;
		QHash<RIconButton *, RMessageBox::StandardButton> buttHash;
		RMessageBox::StandardButton clickButton;
	};

	void RMessageBoxPrivate::initWidget()
	{
		QVBoxLayout * mainLayout = new QVBoxLayout;
		mainLayout->setContentsMargins(1, 0, 1, 1);
		mainLayout->setSpacing(0);

		//    toolBar = new ToolBar();
		//    toolBar->setObjectName("Widget_ToolBarWidget");
		//    toolBar->setFixedHeight(ABSTRACT_TOOL_BAR_HEGIHT);
		//    toolBar->setToolFlags(ToolBar::TOOL_MESSAGEBOX);
		//    toolBar->setWindowIcon(RSingleton<ImageManager>::instance()->getWindowIcon(ImageManager::WHITE,ImageManager::ICON_SYSTEM,ImageManager::ICON_16));
		//    toolBar->setWindowTitle(QObject::tr("Information"));

		//    QObject::connect(toolBar,SIGNAL(closeWindow()),q_ptr,SLOT(close()));

		iconLabel = new RIconLabel();
		iconLabel->setCorner(false);
		iconLabel->setEnterCursorChanged(false);
		iconLabel->setTransparency(true);
		iconLabel->setFixedSize(40, 40);

		contentLabel = new QLabel();
		contentLabel->setWordWrap(true);
		contentLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		contentLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		QWidget * contentWidget = new QWidget();
		contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		QHBoxLayout * contentLayout = new QHBoxLayout();

		contentLayout->addWidget(iconLabel);
		contentLayout->addWidget(contentLabel);

		contentWidget->setLayout(contentLayout);

		bottomWidget = new QWidget;
		bottomWidget->setFixedHeight(40);
		bottomWidget->setMinimumSize(100, 30);

		//    mainLayout->addWidget(toolBar);
		mainLayout->addWidget(contentWidget);
		mainLayout->addWidget(bottomWidget);

		QWidget * mainWidget = new QWidget();
		mainWidget->setObjectName("AbstractWidget_ContentWidget");
		mainWidget->setLayout(mainLayout);

		q_ptr->enableToolButtonArea(false);
		q_ptr->setContentWidget(mainWidget);
	}

	void RMessageBoxPrivate::setIcon(RMessageBox::Icon type)
	{
		mesType = type;

		switch (type)
		{
		case RMessageBox::Information:
			iconLabel->setPixmap(":/icon/resource/icon/alert_info.png");
			break;
		case RMessageBox::Warning:
			iconLabel->setPixmap(":/icon/resource/icon/alert_warning.png");
			break;

		case RMessageBox::Critical:
			iconLabel->setPixmap(":/icon/resource/icon/aler_error.png");
			break;
		default:
			break;
		}
	}

	void RMessageBoxPrivate::updateButtLayout()
	{
		if (!bottomWidget->layout())
		{
			QHBoxLayout * layout = new QHBoxLayout;
			layout->setContentsMargins(6, 6, 6, 6);
			bottomWidget->setLayout(layout);
		}

		QHBoxLayout * layout = dynamic_cast<QHBoxLayout *>(bottomWidget->layout());

		int i = layout->count() - 1;

		while (i >= 0)
		{
			QLayoutItem * item = layout->takeAt(i);
			if (QWidget *widget = item->widget())
			{
				widget->hide();
			}
			delete item;
			i--;
		}

		for (int j = 0; j < buttList.size(); j++)
		{
			if (j == 0)
			{
				layout->addStretch(1);
			}
			buttList.at(j)->show();
			layout->addWidget(buttList.at(j));
		}
	}

#define MESSAGE_BOX_WIDTH 370
#define MESSAGE_BOX_HEIGHT 200

	RMessageBox::RMessageBox(QWidget *parent) :
		d_ptr(new RMessageBoxPrivate(this)),
		DialogProxy(parent)
	{
		setWindowIcon(QIcon(RSingleton<Base::ImageManager>::instance()->getWindowIcon(Base::ImageManager::NORMAL)));
		QSize size = Base::RUtil::screenSize();
		setGeometry((size.width() - MESSAGE_BOX_WIDTH) / 2, (size.height() - MESSAGE_BOX_HEIGHT) / 2, MESSAGE_BOX_WIDTH, MESSAGE_BOX_HEIGHT);
	}

	RMessageBox::~RMessageBox()
	{
		delete d_ptr;
	}

	void RMessageBox::setWindowTitle(const QString &text)
	{
		setTitle(text);
	}

	void RMessageBox::setText(const QString &text)
	{
		Q_D(RMessageBox);
		d->contentLabel->setText(text);
	}

	QString RMessageBox::text() const
	{
		Q_D(const RMessageBox);
		return d->contentLabel->text();
	}

	void RMessageBox::setIcon(RMessageBox::Icon icon)
	{
		Q_D(RMessageBox);
		d->setIcon(icon);
	}

	RMessageBox::StandardButton RMessageBox::information(QWidget *parent, const QString &title, const QString &text, StandardButtons butts, RMessageBox::StandardButton defaultButt)
	{
		return messagebox(parent, Information, title, text, butts, defaultButt);
	}

	RMessageBox::StandardButton RMessageBox::warning(QWidget *parent, const QString &title, const QString &text, StandardButtons butts, RMessageBox::StandardButton defaultButt)
	{
		return messagebox(parent, Warning, title, text, butts, defaultButt);
	}

	RMessageBox::StandardButton RMessageBox::clickedButton() const
	{
		Q_D(const RMessageBox);
		return d->clickButton;
	}

	//void RMessageBox::paintEvent(QPaintEvent *)
	//{
	//    QPainterPath path;
	//    path.setFillRule(Qt::WindingFill);
	//    path.addRect(WINDOW_MARGIN_WIDTH, WINDOW_MARGIN_WIDTH, this->width()-WINDOW_MARGIN_WIDTH * 2, this->height()- WINDOW_MARGIN_WIDTH * 2);
	//    QPainter painter(this);
	//    painter.setRenderHint(QPainter::Antialiasing, true);
	//    painter.fillPath(path, QBrush(Qt::white));

	//    QColor color(0, 0, 0, 50);
	//    for(int i = 0; i< WINDOW_MARGIN_WIDTH; i++)
	//    {
	//        QPainterPath path;
	//        path.setFillRule(Qt::WindingFill);
	//        path.addRect(WINDOW_MARGIN_WIDTH-i, WINDOW_MARGIN_WIDTH-i, this->width()-(WINDOW_MARGIN_WIDTH-i)*2, this->height()-(WINDOW_MARGIN_WIDTH-i)*2);
	//        color.setAlpha(150 - qSqrt(i)*50);
	//        painter.setPen(color);
	//        painter.drawPath(path);
	//    }
	//}

	void RMessageBox::closeEvent(QCloseEvent *event)
	{
		event->accept();
	}

	void RMessageBox::respButtonClicked()
	{
		Q_D(RMessageBox);
		RIconButton * button = dynamic_cast<RIconButton *>(QObject::sender());
		if (button)
		{
			d->clickButton = d->buttHash.value(button);
			close();
		}
	}

	RIconButton *RMessageBox::addButton(RMessageBox::StandardButton butt)
	{
		Q_D(RMessageBox);
		RIconButton * button = new RIconButton(d->bottomWidget);
		button->setMinimumSize(60, 26);
		button->setText(standardButtText(butt));
		connect(button, SIGNAL(clicked()), this, SLOT(respButtonClicked()));

		d->buttHash.insert(button, butt);
		d->buttList.append(button);
		d->updateButtLayout();

		return button;
	}

	QString RMessageBox::standardButtText(RMessageBox::StandardButton butt)
	{
		if (butt)
		{
			switch (butt)
			{
			case RMessageBox::Ok:
				return QStringLiteral("确定");
			case RMessageBox::Save:
				return QCoreApplication::translate("QPlatformTheme", "Save");
			case RMessageBox::SaveAll:
				return QCoreApplication::translate("QPlatformTheme", "SaveAll");
			case RMessageBox::Open:
				return QCoreApplication::translate("QPlatformTheme", "Open");
			case RMessageBox::Yes:
				return QStringLiteral("确定");
			case RMessageBox::YesToAll:
				return QCoreApplication::translate("QPlatformTheme", "YesToAll");
			case RMessageBox::No:
				return QStringLiteral("取消");;
			case RMessageBox::NoToAll:
				return QCoreApplication::translate("QPlatformTheme", "NoToAll");
			case RMessageBox::Abort:
				return QCoreApplication::translate("QPlatformTheme", "Abort");
			case RMessageBox::Retry:
				return QCoreApplication::translate("QPlatformTheme", "Retry");
			case RMessageBox::Ignore:
				return QCoreApplication::translate("QPlatformTheme", "Ignore");
			case RMessageBox::Close:
				return QCoreApplication::translate("QPlatformTheme", "Close");
			case RMessageBox::Cancel:
				return QCoreApplication::translate("QPlatformTheme", "Cancel");
			case RMessageBox::Discard:
				return QCoreApplication::translate("QPlatformTheme", "Discard");
			case RMessageBox::Help:
				return QCoreApplication::translate("QPlatformTheme", "Help");
			case RMessageBox::Apply:
				return QCoreApplication::translate("QPlatformTheme", "Apply");
			case RMessageBox::Reset:
				return QCoreApplication::translate("QPlatformTheme", "Reset");
			case RMessageBox::RestoreDefaults:
				return QCoreApplication::translate("QPlatformTheme", "RestoreDefaults");
			default:
				break;
			}
		}
		return QString();
	}

	RMessageBox::StandardButton RMessageBox::messagebox(QWidget *parent, RMessageBox::Icon type, const QString &title, const QString &text, StandardButtons buttons, StandardButton)
	{
		RMessageBox msgBox(parent);

		msgBox.setWindowTitle(title);
		msgBox.setText(text);
		msgBox.setIcon(type);

		uint mask = RMessageBox::FirstButton;
		while (mask <= RMessageBox::LastButton)
		{
			uint sb = buttons & mask;
			mask <<= 1;
			if (!sb)
				continue;
			msgBox.addButton((RMessageBox::StandardButton)sb);
		}

		if (msgBox.exec() == -1)
		{
			return RMessageBox::Cancel;
		}

		return  msgBox.clickedButton();
	}

} //namespace Base 
