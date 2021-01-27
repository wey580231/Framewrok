/*!
 * @brief     工具类
 * @details	  提供与当前课题相关的工具操作类
 * @author    wey
 * @version   1.0
 * @date      21.01.14 14:54:25
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QObject>

#include <base\selfwidget\iconbutton.h>
#include <base\selfwidget\rmessagebox.h>

namespace Related {

	class Util : public QObject
	{
	public:
		Util(QObject *parent = nullptr);
		~Util();

		static Base::RIconButton * createButt(QString icon, QString text);

		static Base::RMessageBox::StandardButton showQuestion(QWidget * parent,QString questionContent);
		static Base::RMessageBox::StandardButton showInformation(QWidget * parent,QString infoContent);
		static Base::RMessageBox::StandardButton showWarning(QWidget * parent,QString warnContent);


	};

} //namespace Related 