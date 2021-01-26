/*!
 *  @brief     自定义list列表窗口
 *  @details   
 *  @author    wey
 *  @version   1.0
 *  @date      2021.01.06
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#pragma once

#include <QWidget>
#include <qabstractbutton.h>
#include <qbuttongroup.h>

#include <base\selfwidget\iconbutton.h>

class QVBoxLayout;

namespace Related {

	class RListWidget;

	class RListWidgetItem : public Base::RIconButton 
	{
		Q_OBJECT

	public:
		explicit RListWidgetItem(QWidget * parent = nullptr);
		RListWidgetItem();

		QSize minimumSizeHint() const;
		void setPageId(int pageId);
		int pageId()const { return m_pageId; }

	protected:
		void paintEvent(QPaintEvent * e) override;
		void enterEvent(QEvent * event) override;
		void leaveEvent(QEvent * event) override;

	private:
		void setExpandModel(bool flag);

	private:
		bool m_mouseEnter;
		bool m_expandModel;			// 是否为展开模式，默认为true
		int m_pageId;

		friend class RListWidget;
	};

	class RListWidget : public QWidget
	{
		Q_OBJECT

	public:
		RListWidget(QWidget *parent);
		~RListWidget();

		void addItem(int pageId,QString text,QIcon icon);
		void addItem(RListWidgetItem * item);

		void setExpanded(bool enable);
		void setCurrentIndex(int index);

		/*!
		 * @brief 获取当前选中项对应的页面ID
		 * @return -1:未添加项；>= 0 : 被选中的页面ID
		 */
		int currentIndex() const;

	signals:
		void currentIndexChanged(int pageId);

	private slots:
		void indexChanged(QAbstractButton * butt);

	private:
		void init();

	private:
		QList<RListWidgetItem * > m_items;
		QVBoxLayout * m_mainLayout;
		QButtonGroup * m_buttGroup;
		bool m_expandModel;			// 是否为展开模式，默认为true
	};

} //namespace Related 