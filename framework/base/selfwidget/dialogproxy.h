/*!
 *  @brief     模态对话框代理
 *  @details   1.作为创建交互式弹出框的模板，将窗口的主体延迟至子类设定。@n
 *             2.主体内容区A：用于容纳子类窗体，调用setContentWidget()可设置； @n
 *             3.按钮区C：(可选操作)自定义窗体按钮操作，设置后通过respButtClicked()方法可获取点击的按钮 @n
 *             4.扩展工具区B：通过setExtandToolWidget()可设置自定义该区域面板 @n
 *             5.Title：通过setTitle()设置窗体标题 @n
 *            |--[Title]----------------------------[x]|
 *            |                                        |
 *            |                                        |
 *            |                                        |
 *            |               主体内容区A                |
 *            |                                        |
 *            |                                        |
 *            |                                        |
 *            ------------------------------------------
 *            |       扩展工具区 B      |    按钮区C   |
 *            ------------------------------------------
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.31
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef DIALOGPROXY_H
#define DIALOGPROXY_H

#include <QDialog>
#include <QAbstractButton>
#include <QLayout>

#include "../base_global.h"
#include "../macros.h"

class QLabel;

namespace Base {

	class TitleLayout;

	class DialogTitleBar : public QWidget
	{
		Q_OBJECT
	public:
		explicit DialogTitleBar(QWidget * parent);
		~DialogTitleBar();

		enum ButtonRole {
			IconLabel = 0x00, TitleLabel, MinButton, MaxButton, CloseButton, RoleCount
		};

		void setTitle(QString text);

	protected:
		bool eventFilter(QObject *watched, QEvent *event);

	signals:
		void newOffsetPos(QPoint point);
		void widgetClose();

	private:
		void updateButtons();

	private:
		QWidget * titleContent;
		QLabel * titleLabel;

		QPoint mouseStartPoint;     /*!< 鼠标按下位置 */
		bool mouseMoveable;         /*!< 鼠标点下的位置是否支持移动 */

		TitleLayout * layout;
	};

	class TitleLayout : public QLayout
	{
	public:
		explicit TitleLayout(QWidget * parent = 0);

		void addWidget(DialogTitleBar::ButtonRole role, QWidget * w);
		QWidget * getWidget(DialogTitleBar::ButtonRole role) const;

		void addItem(QLayoutItem * item);
		void setGeometry(const QRect &geometry);
		QLayoutItem *itemAt(int index)const;
		QLayoutItem *takeAt(int index);
		int count() const;
		QSize minimumSize();
		QSize sizeHint() const;

	private:
		enum SizeType { MinimumSize, SizeHint };
		QSize calculateSize(SizeType sizeType) const;

	private:
		QVector<QWidgetItem *> items;
	};

	class DialogProxyPrivate;

	class BASESHARED_EXPORT DialogProxy : public QDialog
	{
		Q_OBJECT
			Q_DECLARE_PRIVATE(DialogProxy)
	public:
		explicit DialogProxy(QWidget * parent = 0);
		~DialogProxy();

		enum StandardButton
		{
			NoButton = 0x00000000,
			Ok = 0x00000400,
			Save = 0x00000800,
			SaveAll = 0x00001000,
			Open = 0x00002000,
			Yes = 0x00004000,
			YesToAll = 0x00008000,
			No = 0x00010000,
			NoToAll = 0x00020000,
			Abort = 0x00040000,
			Retry = 0x00080000,
			Ignore = 0x00100000,
			Close = 0x00200000,
			Cancel = 0x00400000,
			Discard = 0x00800000,
			Help = 0x01000000,
			Apply = 0x02000000,
			Reset = 0x04000000,
			RestoreDefaults = 0x08000000,

			FirstButton = Ok,                // internal
			LastButton = RestoreDefaults,   // internal

			YesAll = YesToAll,          // obsolete
			NoAll = NoToAll,           // obsolete
		};
		Q_DECLARE_FLAGS(StandardButtons, StandardButton)
		Q_DECLARE_FRIEND_FLAGS(StandardButtons)

		void setButton(StandardButtons butts);
		void setButton(StandardButton butt, QObject * reiver, const char * slot);
		void setContentWidget(QWidget * widget);
		void enableToolButtonArea(bool flag = true);

		void setExtandToolWidget(QWidget * widget);
		void setTitle(QString content);

	signals:
		void buttClicked(StandardButton butt);

	protected:
		virtual void respButtClicked(StandardButton butt);

	private slots:
		void respButtonClicked();
		void udpatePos(QPoint offsetPos);

	private:
		void addButton(DialogProxy::StandardButton buttType, QObject * reiver = nullptr, const char * slot = NULL);
		QString standardButtText(DialogProxy::StandardButton butt);

	private:
		DialogProxyPrivate * d_ptr;
	};

} //namespace Base 

#endif // DIALOGPROXY_H
