/*!
 * @brief     音频时域波形缩略图  
 * @details   1.显示当前打开音频的波形图
			  2.支持设置显示范围查看
 * @author    wey
 * @version   1.0
 * @date      2021.02.01 17:08:25
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QWidget>

namespace Related {

	class TinyWavGraph : public QWidget
	{
		Q_OBJECT

	public:
		TinyWavGraph(QWidget * parent = nullptr);
		~TinyWavGraph();

	protected:
		bool eventFilter(QObject * watched, QEvent * event);

	private:
		void init();

	private:
		QWidget * m_waveGraph;
	};

} //namespace Related 