#include "dialogproxy.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QPushButton>
#include <QVariant>
#include <QHash>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <QStyle>
#include <QStyleOptionDockWidget>
#include <QPainter>
#include <QStyleOptionToolButton>

#include "rbutton.h"

namespace Base {

	TitleBarButton::TitleBarButton(QWidget *widget) :QAbstractButton(widget)
	{
		setFocusPolicy(Qt::NoFocus);
	}

	QSize TitleBarButton::sizeHint() const
	{
		ensurePolished();

		int size = 2 * style()->pixelMetric(QStyle::PM_TitleBarHeight, 0, this);
		if (!icon().isNull()) {
			int iconSize = style()->pixelMetric(QStyle::PM_SmallIconSize, 0, this);
			QSize sz = icon().actualSize(QSize(iconSize, iconSize));
			size += qMax(sz.width(), sz.height());
		}

		return QSize(size, size);
	}

	void TitleBarButton::enterEvent(QEvent *event)
	{
		if (isEnabled()) update();
		QAbstractButton::enterEvent(event);
	}

	void TitleBarButton::leaveEvent(QEvent *event)
	{
		if (isEnabled()) update();
		QAbstractButton::leaveEvent(event);
	}

	void TitleBarButton::paintEvent(QPaintEvent *)
	{
		QPainter p(this);

		QStyleOptionToolButton opt;
		opt.init(this);
		opt.state |= QStyle::State_AutoRaise;

		if (style()->styleHint(QStyle::SH_DockWidget_ButtonsHaveFrame, 0, this))
		{
			if (isEnabled() && underMouse() && !isChecked() && !isDown())
				opt.state |= QStyle::State_Raised;
			if (isChecked())
				opt.state |= QStyle::State_On;
			if (isDown())
				opt.state |= QStyle::State_Sunken;
			style()->drawPrimitive(QStyle::PE_PanelButtonTool, &opt, &p, this);
		}

		opt.icon = icon();
		opt.subControls = 0;
		opt.activeSubControls = 0;
		opt.features = QStyleOptionToolButton::None;
		opt.arrowType = Qt::NoArrow;
		int size = style()->pixelMetric(QStyle::PM_SmallIconSize, 0, this);
		opt.iconSize = QSize(size, size);
		style()->drawComplexControl(QStyle::CC_ToolButton, &opt, &p, this);
	}


	DialogTitleBar::DialogTitleBar(QWidget *parent)
	{
		Q_UNUSED(parent)
			setFixedHeight(22);

		titleContent = new QWidget(this);

		layout = new TitleLayout(titleContent);

		titleLabel = new QLabel(titleContent);
		titleLabel->setObjectName("titlebar_titlelabel");
		titleLabel->setStyleSheet("background-color:rgba(53,194,202,210);padding-left:3px;text-aligcn:");
		titleLabel->installEventFilter(this);
		layout->addWidget(DialogTitleBar::TitleLabel, titleLabel);

		QAbstractButton *closeButt = new TitleBarButton(titleContent);
		closeButt->setFixedSize(22, 22);
		closeButt->setObjectName(QLatin1String("title_closebutton"));
		connect(closeButt, SIGNAL(clicked()), this, SIGNAL(widgetClose()));
		layout->addWidget(DialogTitleBar::CloseButton, closeButt);

		titleContent->setLayout(layout);

		updateButtons();

		QHBoxLayout * mainLayout = new QHBoxLayout;
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(titleContent);
		this->setLayout(mainLayout);
	}

	DialogTitleBar::~DialogTitleBar()
	{

	}

	void DialogTitleBar::setTitle(QString text)
	{
		titleLabel->setText(text);
	}

	bool DialogTitleBar::eventFilter(QObject *watched, QEvent *event)
	{
		if (watched == titleLabel)
		{
			switch (event->type()) {
			case QEvent::MouseButtonPress:
			{
				QMouseEvent * eve = dynamic_cast<QMouseEvent *>(event);
				if (eve) {
					mouseStartPoint = eve->pos();
					mouseMoveable = true;
				}
			}
			break;
			case QEvent::MouseMove:
			{
				QMouseEvent * eve = dynamic_cast<QMouseEvent *>(event);
				if (eve) {
					QPoint offsetPos = eve->pos() - mouseStartPoint;
					emit newOffsetPos(offsetPos);
					setCursor(Qt::ClosedHandCursor);
				}
			}
			break;
			case QEvent::MouseButtonRelease:
			{
				mouseMoveable = false;
				setCursor(Qt::OpenHandCursor);
			}
			break;
			case QEvent::Enter: {
				setCursor(Qt::OpenHandCursor);
			}
								break;
			case QEvent::Leave: {
				setCursor(Qt::ArrowCursor);
			}
								break;
			default:
				break;
			}
		}
		return QWidget::eventFilter(watched, event);
	}

	void DialogTitleBar::updateButtons()
	{
		QStyleOptionDockWidget option;
		option.initFrom(this);
		option.rect = this->rect();
		option.title = titleLabel->text();
		option.closable = true;
		option.movable = true;

		bool canClose = true;

		QAbstractButton *button = qobject_cast<QAbstractButton*>(layout->getWidget(DialogTitleBar::CloseButton));
		button->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton, &option, this));
		button->setVisible(canClose);

		setAttribute(Qt::WA_ContentsPropagated, canClose);
	}

	class DialogProxyPrivate : public QObject
	{
		Q_DECLARE_PUBLIC(DialogProxy)
	public:
		DialogProxyPrivate(DialogProxy * q) :q_ptr(q), extandToolWidget(NULL), buttonContainWidget(NULL),
			enableToolButton(true) {
			initView();
		}
		~DialogProxyPrivate() {
			delete mainWidget;
		}

		void initView();
		void updateButtLayout();
		void createButtLayout();

	private:
		DialogProxy * q_ptr;

		QWidget * mainWidget;
		QWidget * contentWidget;                /*!< 中心显示区 */
		QWidget * buttonContainWidget;          /*!< 按钮区 */
		QWidget * extandToolWidget;             /*!< 扩展工具区 */
		DialogTitleBar *  titleBar;

		bool enableToolButton;                  /*!< 是否将buttonContainWidget 加入布局，默认为true */

		QList<RButton *> buttList;
		QHash<RButton *, DialogProxy::StandardButton> buttHash;
	};

	void DialogProxyPrivate::initView()
	{
		mainWidget = new QWidget(q_ptr);

		titleBar = new DialogTitleBar(q_ptr);
		connect(titleBar, SIGNAL(newOffsetPos(QPoint)), q_ptr, SLOT(udpatePos(QPoint)));
		connect(titleBar, SIGNAL(widgetClose()), q_ptr, SLOT(close()));

		contentWidget = new QWidget(mainWidget);
		contentWidget->setObjectName("mainWidget");
		contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		buttonContainWidget = new QWidget(contentWidget);
		buttonContainWidget->setFixedHeight(35);

		QVBoxLayout * mainLayout = new QVBoxLayout;
		mainLayout->setContentsMargins(1, 1, 1, 1);
		mainLayout->setSpacing(2);

		mainLayout->addWidget(titleBar);
		mainLayout->addWidget(contentWidget);

		mainWidget->setLayout(mainLayout);

		QHBoxLayout * layout = new QHBoxLayout;
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(mainWidget);
		q_ptr->setLayout(layout);
	}

	/*!
	 * @brief 更新按钮区域布局
	 * @details 需考虑ExtandToolWidget带来的布局影响
	 */
	void DialogProxyPrivate::updateButtLayout()
	{
		createButtLayout();

		QHBoxLayout * layout = dynamic_cast<QHBoxLayout *>(buttonContainWidget->layout());

		QLayoutItem * extendToolItem = NULL;
		int i = layout->count() - 1;
		while (i >= 0)
		{
			QLayoutItem * item = layout->takeAt(i);
			QWidget *widget = item->widget();
			bool canDeleted = true;
			if (widget) {
				RButton * rbutt = dynamic_cast<RButton *>(widget);
				if (rbutt) {
					rbutt->hide();
				}
				else {
					canDeleted = false;
					extendToolItem = item;
				}
			}
			if (canDeleted)
				delete item;
			i--;
		}

		if (extendToolItem)
			layout->addItem(extendToolItem);

		for (int j = 0; j < buttList.size(); j++)
		{
			if (j == 0)
				layout->addStretch(1);

			buttList.at(j)->show();
			layout->addWidget(buttList.at(j));
		}
	}

	void DialogProxyPrivate::createButtLayout()
	{
		if (!buttonContainWidget->layout()) {
			QHBoxLayout * layout = new QHBoxLayout;
			layout->setContentsMargins(6, 3, 3, 3);
			buttonContainWidget->setLayout(layout);
		}
	}


	DialogProxy::DialogProxy(QWidget *parent) :QDialog(parent),
		d_ptr(new DialogProxyPrivate(this))
	{
		Qt::WindowFlags  flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;
		flags |= Qt::FramelessWindowHint;
		setAttribute(Qt::WA_TranslucentBackground);
		setWindowFlags(flags);
	}

	DialogProxy::~DialogProxy()
	{
		delete d_ptr;
	}

	/*!
	 * @brief 在底部按钮区添加按钮
	 * @param[in]  butts 待添加的按钮or集合
	 */
	void DialogProxy::setButton(int butts)
	{
		if (butts <= 0)
			return;

		uint mask = DialogProxy::FirstButton;
		while (mask <= DialogProxy::LastButton)
		{
			uint sb = butts & mask;
			mask <<= 1;
			if (!sb)
				continue;
			addButton((DialogProxy::StandardButton)sb);
		}
	}

	/*!
	 * @brief 在按钮区添加按钮，并绑定槽函数
	 * @param[in] butts 待添加的单个按钮
	 * @param[in] slot 槽函数
	 */
	void DialogProxy::setButton(int butt, QObject *reiver, const char *slot)
	{
		if (butt <= 0 || reiver == nullptr || slot == NULL)
			return;
		StandardButton button = static_cast<StandardButton>(butt);
		if (button <= DialogProxy::LastButton && button >= DialogProxy::FirstButton)
			addButton(button, reiver, slot);
	}

	/*!
	 * @brief 设置中心widget
	 */
	void DialogProxy::setContentWidget(QWidget *widget)
	{
		Q_D(DialogProxy);
		if (widget == nullptr)
			return;

		if (!d->contentWidget->layout()) {
			QVBoxLayout * layout = new QVBoxLayout;
			layout->setContentsMargins(0, 0, 0, 0);

			layout->addWidget(widget);
			if (d->enableToolButton)
				layout->addWidget(d->buttonContainWidget);

			d->contentWidget->setLayout(layout);
		}
	}

	/*!
	* @brief 是否将buttonContainWidget加入布局，默认为true
	* @warning 此方法需要在setContentWidget调用之前调用
	* @param[in] flag
	*/
	void DialogProxy::enableToolButtonArea(bool flag)
	{
		Q_D(DialogProxy);
		d->enableToolButton = flag;
	}

	/*!
	 * @brief 设置自定义扩展工具窗口
	 * @param[in]  widget  自定义扩展窗口
	 */
	void DialogProxy::setExtandToolWidget(QWidget *widget)
	{
		Q_D(DialogProxy);
		if (widget == NULL)
			return;

		d->createButtLayout();
		QHBoxLayout * layout = dynamic_cast<QHBoxLayout *>(d->buttonContainWidget->layout());
		//[1]将现有的扩展工具区移除
		int index = 0;
		bool findIndex = false;
		if (d->extandToolWidget) {
			while (index < layout->count()) {
				QWidget * tmpWidget = layout->itemAt(index)->widget();
				if (tmpWidget && tmpWidget == d->extandToolWidget) {
					findIndex = true;
					break;
				}
				index++;
			}

			if (findIndex) {
				QLayoutItem * item = layout->takeAt(index);
				delete item;
			}
		}

		layout->insertWidget(index, widget);
		d->extandToolWidget = widget;
	}

	/*!
	 * @brief 响应按钮点击
	 * @details 子类应覆写此类，用于响应点击操作
	 * @param[in]  butt 发生点击按钮的类型
	 */
	void DialogProxy::respButtClicked(DialogProxy::StandardButton /*butt*/)
	{

	}

	void DialogProxy::setTitle(QString content)
	{
		Q_D(DialogProxy);
		d->titleBar->setTitle(content);
	}

	void DialogProxy::addButton(DialogProxy::StandardButton buttType, QObject * reiver, const char * slot)
	{
		Q_D(DialogProxy);
		RButton * button = new RButton(d->buttonContainWidget);
		if (reiver && slot)
			connect(button, SIGNAL(clicked(bool)), reiver, slot);
		button->setProperty("buttType", buttType);
		button->setText(standardButtText(buttType));
		connect(button, SIGNAL(clicked()), this, SLOT(respButtonClicked()));

		d->buttHash.insert(button, buttType);
		d->buttList.append(button);
		d->updateButtLayout();
	}

	/*!
	 * @brief 处理按钮点击
	 * @details 按钮发生点击时，存在两种处理当前按钮响应的操作。
	 *          【1】覆写respButtClicked(StandardButton)方法；
	 *          【2】响应buttClicked(StandardButton)信号
	 */
	void DialogProxy::respButtonClicked()
	{
		RButton * butt = dynamic_cast<RButton *>(QObject::sender());
		if (butt) {
			StandardButton but = static_cast<StandardButton>(butt->property("buttType").toInt());
			respButtClicked(but);
			emit buttClicked(but);
		}
	}

	void DialogProxy::udpatePos(QPoint offsetPos)
	{
		this->move(pos() + offsetPos);
	}

	/*!
	 * @brief 获取对应按钮的文字描述
	 * @param[in]  butt 按钮的类型
	 * @return 对应文字描述
	 */
	QString DialogProxy::standardButtText(DialogProxy::StandardButton butt)
	{
		if (butt)
		{
			switch (butt)
			{
			case DialogProxy::Ok:
				return QStringLiteral("确定");
			case DialogProxy::Save:
				return QCoreApplication::translate("QPlatformTheme", "Save");
			case DialogProxy::SaveAll:
				return QCoreApplication::translate("QPlatformTheme", "SaveAll");
			case DialogProxy::Open:
				return QCoreApplication::translate("QPlatformTheme", "Open");
			case DialogProxy::Yes:
				return QStringLiteral("确定");
			case DialogProxy::YesToAll:
				return QCoreApplication::translate("QPlatformTheme", "YesToAll");
			case DialogProxy::No:
				return QStringLiteral("取消");
			case DialogProxy::NoToAll:
				return QCoreApplication::translate("QPlatformTheme", "NoToAll");
			case DialogProxy::Abort:
				return QCoreApplication::translate("QPlatformTheme", "Abort");
			case DialogProxy::Retry:
				return QCoreApplication::translate("QPlatformTheme", "Retry");
			case DialogProxy::Ignore:
				return QCoreApplication::translate("QPlatformTheme", "Ignore");
			case DialogProxy::Close:
				return QCoreApplication::translate("QPlatformTheme", "Close");
			case DialogProxy::Cancel:
				return QCoreApplication::translate("QPlatformTheme", "Cancel");
			case DialogProxy::Discard:
				return QCoreApplication::translate("QPlatformTheme", "Discard");
			case DialogProxy::Help:
				return QCoreApplication::translate("QPlatformTheme", "Help");
			case DialogProxy::Apply:
				return QCoreApplication::translate("QPlatformTheme", "Apply");
			case DialogProxy::Reset:
				return QCoreApplication::translate("QPlatformTheme", "Reset");
			case DialogProxy::RestoreDefaults:
				return QCoreApplication::translate("QPlatformTheme", "RestoreDefaults");
			default:
				break;
			}
		}
		return QString();
	}

	TitleLayout::TitleLayout(QWidget *parent) :QLayout(parent), items(DialogTitleBar::RoleCount, NULL)
	{

	}

	void TitleLayout::addWidget(DialogTitleBar::ButtonRole role, QWidget *w)
	{
		QWidget * tmpWidget = getWidget(role);
		if (tmpWidget) {
			tmpWidget->hide();
			removeWidget(w);
		}

		if (w) {
			QWidgetItem * item = new QWidgetItem(w);
			items[role] = item;
			w->show();
		}
		else {
			items[role] = NULL;
		}
	}

	QWidget *TitleLayout::getWidget(DialogTitleBar::ButtonRole role) const
	{
		QLayoutItem * item = items.at(role);
		return item == 0 ? 0 : item->widget();
	}

	void TitleLayout::addItem(QLayoutItem *item)
	{
		Q_UNUSED(item)
			qWarning() << "please use DockLayout::addWidget()";
		return;
	}

	void TitleLayout::setGeometry(const QRect &geometry)
	{
		int titleWidth = geometry.width();
		int leftPos = titleWidth;
		int rightPos = 0;
		int itemSpace = 1;

		//closebutton
		if (QLayoutItem * item = items[DialogTitleBar::CloseButton]) {
			if (!item->isEmpty()) {
				leftPos -= item->widget()->width();
				item->setGeometry(QRect(leftPos, 0, item->widget()->width(), item->widget()->height()));
			}
		}

		//maxbutton
		if (QLayoutItem * item = items[DialogTitleBar::MaxButton]) {
			if (!item->isEmpty()) {
				leftPos -= item->widget()->width();
				leftPos -= itemSpace;
				item->setGeometry(QRect(leftPos, 0, item->widget()->width(), item->widget()->height()));
			}
		}

		//minbutton
		if (QLayoutItem * item = items[DialogTitleBar::MinButton]) {
			if (!item->isEmpty()) {
				leftPos -= item->widget()->width();
				leftPos -= itemSpace;
				item->setGeometry(QRect(leftPos, 0, item->widget()->width(), item->widget()->height()));
			}
		}

		//iconlabel
		if (QLayoutItem * item = items[DialogTitleBar::IconLabel]) {
			if (!item->isEmpty()) {
				rightPos += item->widget()->width();
				rightPos += itemSpace;
				item->setGeometry(QRect(0, 0, item->widget()->width(), item->widget()->height()));
			}
		}

		//TitleLabel
		if (QLayoutItem * item = items[DialogTitleBar::TitleLabel]) {
			if (!item->isEmpty()) {
				item->setGeometry(QRect(rightPos, 0, leftPos - rightPos, geometry.height()));
			}
		}
	}

	QLayoutItem *TitleLayout::itemAt(int index) const
	{
		int cnt = 0;
		for (int i = 0; i < items.count(); ++i) {
			QLayoutItem *item = items.at(i);
			if (item == 0)
				continue;
			if (index == cnt++)
				return item;
		}
		return 0;
	}

	QLayoutItem *TitleLayout::takeAt(int index)
	{
		int j = 0;
		for (int i = 0; i < items.count(); ++i) {
			QLayoutItem *item = items.at(i);
			if (item == 0)
				continue;

			if (index == j)
			{
				items[i] = 0;
				invalidate();
				return item;
			}
			++j;
		}
		return 0;
	}

	int TitleLayout::count() const
	{
		int result = 0;
		for (int i = 0; i < items.count(); ++i) {
			if (items.at(i))
				++result;
		}
		return result;
	}

	QSize TitleLayout::minimumSize()
	{
		return calculateSize(MinimumSize);
	}

	QSize TitleLayout::sizeHint() const
	{
		return calculateSize(SizeHint);
	}

	QSize TitleLayout::calculateSize(TitleLayout::SizeType sizeType) const
	{
		QSize totalSize;

		for (int i = 0; i < items.size(); ++i) {
			QWidgetItem  * item = items.at(i);
			if (item == NULL || item->isEmpty())
				continue;
			QSize itemSize;

			if (sizeType == MinimumSize) {
				itemSize = item->widget()->minimumSize();
			}
			else {
				itemSize = item->widget()->sizeHint();
			}

			totalSize.rheight() += itemSize.height();
		}

		return totalSize;
	}

}//namespace Base 
