/*!
 * @brief	 任务图片上传界面    
 *  @details  鼠标点击上传
 * @author    yzg
 * @version   1.0
 * @date      2021.01.22 18:38:23
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 
#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QFileDialog>
#include <QPainter>
#include <QPixmap>

namespace Related {

	class TaskPictureUploadWidget : public QWidget
	{
		Q_OBJECT

	public:
		TaskPictureUploadWidget(QWidget *parent = nullptr);
		~TaskPictureUploadWidget();

	private :
		//void signalPictureUploadPath(QString path);


	protected:
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);

	private:
		void init();

	private:
		QString  m_picturePath;				/*!< 图片路径 */
	};

}//namespace Related 
