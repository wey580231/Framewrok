/*!
 * @brief     加载等待提示窗口   
 * @author    wey
 * @version   1.0
 * @date      2021.01.29 09:08:46
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>
class QTimer;

#include <base\selfwidget\dialogproxy.h>

namespace Related {

	class LoadingDialog : public Base::DialogProxy
	{
		Q_OBJECT

	public:
		LoadingDialog(QWidget *parent = nullptr);
		~LoadingDialog();

		void showMe(QWidget * parent = nullptr);
		void hideMe();

	protected:
		bool eventFilter(QObject * obj, QEvent * event);

	private slots:
		void switchImage();

	private:
		void init();

	private:
		QWidget * m_mainWidget;

		QList<QPixmap> m_imageList;
		QTimer * m_switchTimer;
		int m_imageIndex;
		bool m_accending;
	};

} //namespace Related 