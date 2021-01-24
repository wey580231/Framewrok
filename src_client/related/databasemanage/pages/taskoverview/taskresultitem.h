/*!
 * @brief     任务结果信息
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
#include <QMouseEvent>

namespace Related {

	class TaskResultItem : public QWidget
	{
		Q_OBJECT

	public:
		TaskResultItem(QWidget *parent = nullptr);
		~TaskResultItem();

		void setPictureIndex(int index);
		int getPictureIndex();

		void setPicturePath(QString path);
		QString getPicturePath();

		void setSelectedstatus(bool status);

		void updateItem();

		

	signals:
		void signalPictureIndex(int index, QString path);

	protected:
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);

	private:
		void  init();

	private:
		int m_pictureIndex;					/*!< 图片索引 */
		QString  m_picturePath;				/*!< 图片路径 */
		bool m_selectedstatus;				/*!< 鼠标选中状态 */
	};

}//namespace Related