/*!
 *  @brief     自定义Tab控件
 *  @details   1.可设置Tab水平排列顺序
			   2.可添加RTabButton或继承子类实现扩展
 *  @author
 *  @version   1.0
 *  @date      2021.01.07
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RTABBAR_H_2020_08_05
#define RTABBAR_H_2020_08_05

#include <QAbstractButton>
#include <QButtonGroup>
#include <QHBoxLayout>

#include "../base_global.h"

namespace Base {

	class AbstractPage;

	class BASESHARED_EXPORT RTabButton : public QAbstractButton
	{
		Q_OBJECT
	public:
		explicit RTabButton(QString text, QWidget *parent = nullptr);
		~RTabButton();

	protected:
		void paintEvent(QPaintEvent * event);
		void enterEvent(QEvent * event);
		void leaveEvent(QEvent * event);

	private:
		bool m_bMouseIn;

		QColor m_normalTextColor;
		QColor m_selectedTextColor;
		QColor m_disableTextColor;
	};

	class BASESHARED_EXPORT RTabBar : public QWidget
	{
		Q_OBJECT
	public:
		explicit RTabBar(QWidget * parent = nullptr);
		~RTabBar();

		enum TabAlignment {
			AlignLeft,
			AlignCenter,
			AlignRight
		};
		void setTabAlignment(TabAlignment align);

		void addTabButton(RTabButton * butt, int index);
		void setTabButtonEnabled(int index, bool flag);

		void setCurrentIndex(int index, bool indexChangeSignal = true);
		int currentIndex();

		int tabVisibleWidth() { return m_tabContainer->width(); }

	protected:
		void paintEvent(QPaintEvent *event);

	signals:
		void currentIndexChanged(int index);

	private slots:
		void respTabChoosed(int index);

	private:
		void init();

	private:
		QWidget * m_tabContainer;
		QButtonGroup * m_buttGroup;
		QList<QAbstractButton *> m_buttList;

		TabAlignment m_tabAlign;
		QHBoxLayout * m_mainLayout;
	};

} //namespace Base

#endif // RTABBAR_H
