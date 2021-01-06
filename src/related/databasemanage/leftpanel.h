#pragma once

#include <QWidget>
#include <qlabel.h>

#include <base\selfwidget\iconbutton.h>
#include <base\util\widgetanimation.h>

namespace Related {

	class RListWidget;

	class LeftPanel : public QWidget
	{
		Q_OBJECT

	public:
		LeftPanel(QWidget *parent);
		~LeftPanel();

	private slots:
		void respLeftPanelExpand(bool checked);

	private:
		void init();

	private:
		RListWidget * m_listWidget;
		
		Base::WidgetAnimation m_animation;

		QLabel * m_prgoramIcon;
		Base::IconButton * m_expandButt;
		int m_expandStateWidth;		//展开状态下宽度
		int m_contractionWidth;		//收缩状态下宽度
	};

} //namespace Related 
