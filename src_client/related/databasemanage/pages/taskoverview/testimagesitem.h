/*!
 * @brief     试验图片
 * @details   该类用于显示结果图片
 * @author    yzg
 * @version   1.0
 * @date      2021.01.21 11:12:55
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>

namespace Related {

	class TestImagesItem : public QWidget
	{
		Q_OBJECT

	public:
		TestImagesItem(QWidget *parent = nullptr);
		~TestImagesItem();

		enum TestImagesType{
			TI_Sketch,				/*!< 简图*/
			TI_Detail				/*!< 详图 */
		};

		void setImagesType(TestImagesType type);
		void setImagesIndex(int index);

	signals:
		void signalSeleteImagesIndex(int index);

	protected:
 		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent * event);

		void enterEvent(QEvent * event);
		void leaveEvent(QEvent * event);
	private:
		void  init();

	private:
		TestImagesType m_imagesType;		/*!< 图片类型 */
		int m_imagesIndex;					/*!< 图片索引 */

		QString  m_imagesPath;				/*!< 图片路径 */
		bool m_imagesSuspension;
	};

}//namespace Related