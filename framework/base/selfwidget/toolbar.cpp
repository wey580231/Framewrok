#include "toolbar.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QStyle>
#include <QList>
#include <QLabel>
#include <QEvent>
#include <QToolTip>
#include <QHelpEvent>
#include <QMetaEnum>

#include "../constants.h"
#include "../actionmanager/actionmanager.h"
#include "../util/rsingleton.h"
#include "../util/imagemanager.h"

namespace Base {

#define WINDOW_ICON_SIZE 16

	/*!
	 * @brief 集合list是否包含t数据
	 * @details the compiler can't inspect list<tNode<T>*> (it doesn't have its definition at this point)
				and so it doesn't know whether list<tNode<T>*>::iterator is either a static field or a type. 需要加上typename
	 * @param[in] list 数据集合
	 * @param[in] t 数据值
	 * @return 是否包含
	 */
	template<class T>
	bool contains(QList<T*> & list, const T * t)
	{
		typename  QList<T*>::iterator iter = list.begin();
		while (iter != list.end())
		{
			if ((*iter) == t)
			{
				return true;
			}
			iter++;
		}

		return false;
	}

	class ToolBarPrivate
	{
		Q_DECLARE_PUBLIC(ToolBar)

	private:
		ToolBarPrivate(ToolBar * q) :q_ptr(q)
		{
			flags = ToolBar::TOOL_NONE;
			iconSize = QSize(16, 16);
			initDialog();
		}

		void initDialog();

		ToolBar::ToolType flags;

		ToolBar * q_ptr;

		QWidget * toolBar;
		QHBoxLayout * toolBarLayout;

		QLabel * iconLabel;
		QLabel * windowTitle;

		RToolButton * minButton;
		RToolButton * maxButton;
		RToolButton * closeButton;

		QLabel * spaceLabel;

		QSize iconSize;

		QList<RToolButton *> toolButts;
	};

	void ToolBarPrivate::initDialog()
	{
		toolBar = new QWidget(q_ptr);
		toolBar->setFixedHeight(Constant::TOOL_BAR_HEIGHT);
		toolBarLayout = new QHBoxLayout();

		iconLabel = new QLabel(toolBar);
		iconLabel->setFixedSize(WINDOW_ICON_SIZE, WINDOW_ICON_SIZE);
		iconLabel->setVisible(flags & ToolBar::TOOL_ICON);
		iconLabel->setPixmap(RSingleton<Base::ImageManager>::instance()->getWindowIcon(Base::ImageManager::WHITE, Base::ImageManager::ICON_SYSTEM, Base::ImageManager::ICON_16));

		windowTitle = new QLabel(toolBar);
		windowTitle->setObjectName("ToolBar_Window_Title");
		windowTitle->setVisible(flags & ToolBar::TOOL_TITLE);

		minButton = new RToolButton(toolBar);
		minButton->setObjectName(Constant::TOOL_MIN);
		minButton->setFixedSize(iconSize);
		minButton->setVisible(flags & ToolBar::TOOL_MIN);
		QObject::connect(minButton, SIGNAL(clicked()), q_ptr, SIGNAL(minimumWindow()));

		maxButton = new RToolButton(toolBar);
		maxButton->setObjectName(Constant::TOOL_MAX);
		maxButton->setFixedSize(iconSize);
		maxButton->setVisible(flags & ToolBar::TOOL_MAX);
		QObject::connect(maxButton, SIGNAL(clicked(bool)), q_ptr, SIGNAL(maxWindow(bool)));

		closeButton = new RToolButton(toolBar);
		closeButton->setObjectName(Constant::TOOL_CLOSE);
		closeButton->setFixedSize(iconSize);
		closeButton->setVisible(flags & ToolBar::TOOL_CLOSE);
		QObject::connect(closeButton, SIGNAL(clicked()), q_ptr, SIGNAL(closeWindow()));

		spaceLabel = new QLabel(toolBar);
		spaceLabel->setObjectName("Tool_Spacer");
		spaceLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
		spaceLabel->setVisible(flags & ToolBar::TOOL_SPACER);

		toolBarLayout->addWidget(iconLabel);
		toolBarLayout->addSpacing(4);
		toolBarLayout->addWidget(windowTitle);

		toolBarLayout->addWidget(spaceLabel);

		toolBarLayout->addWidget(minButton);
		toolBarLayout->addWidget(maxButton);
		toolBarLayout->addWidget(closeButton);

		toolBarLayout->setContentsMargins(5, 0, 0, 0);
		toolBarLayout->setSpacing(0);
		toolBar->setLayout(toolBarLayout);

		QVBoxLayout * layout = new QVBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);
		layout->addWidget(toolBar);

		q_ptr->setLayout(layout);
	}

	ToolBar::ToolBar(QWidget *parent) :
		d_ptr(new ToolBarPrivate(this)),
		QWidget(parent)
	{
		setFixedHeight(Constant::TOOL_BAR_HEIGHT);
	}

	ToolBar::~ToolBar()
	{

	}

	void ToolBar::setObjectName(const QString &name)
	{
		Q_D(ToolBar);
		d->toolBar->setObjectName(name);
	}

	QString ToolBar::objectName() const
	{
		Q_D(const ToolBar);
		return d->toolBar->objectName();
	}

	void ToolBar::setIconSize(QSize size)
	{
		Q_D(ToolBar);
		d->iconSize = size;
	}

	QSize ToolBar::iconSize() const
	{
		Q_D(const ToolBar);
		return d->iconSize;
	}

	void ToolBar::addStretch(int strech)
	{
		Q_D(ToolBar);
		d->toolBarLayout->addStretch(strech);
	}

	void ToolBar::setSpacing(int spacing)
	{
		Q_D(ToolBar);
		d->toolBarLayout->setSpacing(spacing);
	}

	void ToolBar::setWindowIcon(const QString image)
	{
		Q_D(ToolBar);
		if (d->flags & ToolBar::TOOL_ICON)
		{
			d->iconLabel->setPixmap(QPixmap(image));
		}
	}

	void ToolBar::setWindowTitle(const QString &name)
	{
		Q_D(ToolBar);
		if (d->flags & ToolBar::TOOL_TITLE)
		{
			d->windowTitle->setText(name);
		}
	}

	QString ToolBar::windowTitle() const
	{
		Q_D(const ToolBar);
		return d->windowTitle->text();
	}

	/*!
	 * @brief 设置内容边距
	 * @param[in] left 左边距
	 * @param[in] top 上边距
	 * @param[in] right 右边距
	 * @param[in] bottom 下边距
	 * @return 是否插入成功
	 */
	void ToolBar::setContentsMargins(int left, int top, int right, int bottom)
	{
		Q_D(ToolBar);
		d->toolBarLayout->setContentsMargins(left, top, right, bottom);
	}

	/*!
	 * @brief 设置工具栏属性
	 * @details 根据不同的样式类型，控制工具栏中的各个控件的显示状态。
	 * @param[in] types 工具类型
	 * @return 无
	 */
	void ToolBar::setToolFlags(ToolBar::ToolType types)
	{
		Q_D(ToolBar);
		d->flags = types;
		d->minButton->setVisible(types & ToolBar::TOOL_MIN);
		d->maxButton->setVisible(types & ToolBar::TOOL_MAX);
		d->closeButton->setVisible(types & ToolBar::TOOL_CLOSE);
		d->iconLabel->setVisible(types & ToolBar::TOOL_ICON);
		d->windowTitle->setVisible(types & ToolBar::TOOL_TITLE);
		d->spaceLabel->setVisible(types & ToolBar::TOOL_SPACER);
	}

	ToolBar::ToolType ToolBar::flags()
	{
		Q_D(ToolBar);
		return QFlag(d->flags);
	}

	/*!
	 * @brief 向窗口的工具栏中插入工具按钮，默认是自左向右排列
	 * @param[in] toolButton 待插入的工具按钮
	 * @return 是否插入成功
	 */
	bool ToolBar::appendToolButton(RToolButton *toolButton)
	{
		Q_D(ToolBar);
		if (!contains(d->toolButts, toolButton))
		{
			toolButton->setFixedSize(d->iconSize);
			d->toolBarLayout->addWidget(toolButton);
		}
		return true;
	}

	/*!
	 * @brief 插入工具按钮
	 * @param[in] toolButton 待插入的工具按钮
	 * @param[in] ID 若为NULL，则从工具栏的最前面插入；不为NULL，则从指定的控件前面插入
	 * @return 是否插入成功
	 */
	bool ToolBar::insertToolButton(RToolButton *toolButton, const char *ID)
	{
		Q_D(ToolBar);
		if (!contains(d->toolButts, toolButton))
		{
			if (ID == NULL)
			{
				d->toolBarLayout->insertWidget(0, toolButton);
				return true;
			}
			else
			{
				toolButton->setFixedSize(Constant::TOOL_WIDTH, Constant::TOOL_HEIGHT);
				for (int i = 0; i < d->toolBarLayout->count(); i++)
				{
					QWidget * tmpWidget = d->toolBarLayout->itemAt(i)->widget();
					if (tmpWidget && tmpWidget->objectName() == QString(ID))
					{
						d->toolBarLayout->insertWidget(i, toolButton);
						return true;
					}
				}
				d->toolBarLayout->addWidget(toolButton);
			}
		}
		return false;
	}


	class RToolButtonPrivate
	{
		Q_DECLARE_PUBLIC(RToolButton)
	private:
		QString toolTipInfo;
		RToolButton * q_ptr;
	};

	RToolButton::RToolButton(QWidget *parent) :
		d_ptr(new RToolButtonPrivate()),
		QToolButton(parent)
	{
		//    connect(this,SIGNAL(toggled(bool)),this,SLOT(updateItemStyle(bool)));
	}

	RToolButton::~RToolButton()
	{

	}

	void RToolButton::setToolTip(const QString &toolTipInfo)
	{
		Q_D(RToolButton);
		d->toolTipInfo = toolTipInfo;
	}

	void RToolButton::setObjectName(QString objName)
	{
		QToolButton::setObjectName(objName);

		if (icon().isNull())
		{
			QToolButton::setIcon(QIcon(QString(":/icon/resource/icon/%1.png").arg(objName)));
		}
	}

	void RToolButton::setIcon(QString id)
	{
		QToolButton::setIcon(QIcon(QString(":/icon/resource/icon/%1.png").arg(id)));
	}

	void RToolButton::removeIcon()
	{
		if (!icon().isNull())
		{
			QToolButton::setIcon(QIcon());
		}
	}

	bool RToolButton::event(QEvent *e)
	{
		Q_D(RToolButton);
		if (e->type() == QEvent::ToolTip)
		{
			QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);
			QToolTip::showText(helpEvent->globalPos(), d->toolTipInfo);

			return true;
		}
		return QWidget::event(e);
	}

} //namespace Base 