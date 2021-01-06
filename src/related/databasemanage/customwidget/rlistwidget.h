#pragma once

#include <QWidget>
#include <qabstractbutton.h>
#include <qbuttongroup.h>

#include <base\selfwidget\iconbutton.h>

class QVBoxLayout;

namespace Related {

	class RListWidget;

	class RListWidgetItem : public Base::IconButton 
	{
		Q_OBJECT

	public:
		explicit RListWidgetItem(QWidget * parent = nullptr);
		RListWidgetItem();

		QSize minimumSizeHint() const;

	protected:
		void paintEvent(QPaintEvent * e) override;
		void enterEvent(QEvent * event);
		void leaveEvent(QEvent * event);

	private:
		void setExpandModel(bool flag);

	private:
		bool m_mouseEnter;
		bool m_expandModel;			// 是否为展开模式，默认为true

		friend class RListWidget;
	};

	class RListWidget : public QWidget
	{
		Q_OBJECT

	public:
		RListWidget(QWidget *parent);
		~RListWidget();

		void addItem(QString text,QIcon icon);
		void addItem(RListWidgetItem * item);

		void setExpanded(bool enable);
		void setCurrentIndex(int index);

	signals:
		void currentIndexChanged(int index);

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