/*!
 * @brief     单条音频文件对应的原始数据信息   
 * @author    wey
 * @version   1.0
 * @date      2021.01.31 17:19:25
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>

#include <base\selfwidget\dialogproxy.h>

namespace Ui {
	class WavOriginDataWindow;
}

namespace Related {

	class WavOriginDataWindow : public Base::DialogProxy
	{
		Q_OBJECT

	public:
		WavOriginDataWindow(QWidget *parent = nullptr);
		~WavOriginDataWindow();

	private:
		void init();

	private:
		Ui::WavOriginDataWindow * m_ui;

	};

} //namespace Related 