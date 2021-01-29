/*!
 * @brief     自定义仿Win10信息通知   
 * @author    wey
 * @version   1.0
 * @date      2021.01.29 16:23:22
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
 #pragma once

#include <QObject>
#include <QQueue>

#include "../../base_global.h"

namespace Base {

	class Notify;

	class BASESHARED_EXPORT NotifyManager : public QObject
	{
		Q_OBJECT

	public:
		NotifyManager(QObject *parent = nullptr);
		~NotifyManager();

		void information(QString content);
		void warning(QString content);

		void notify(const QString &title, const QString &icon, const QString &content);

		/*! 
		 * @brief 设置通知最大显示数量
		 * @details 不建议通知数量超过6个
		 * @param count >0
		 */
		void setMaxCount(ushort count);

		/*! 
		 * @brief 设置单个通知最长停留时间
		 * @param ms 毫秒
		 */
		void setDisplayTime(int ms);

	private:
		class NotifyData {
		public:
			NotifyData(const QString &icon, const QString &title, const QString &body) :
				icon(icon),title(title),body(body){}

			QString icon;
			QString title;
			QString body;
		};

		void reArrange();
		void showNext();

		QQueue<NotifyData> m_dataQueue;
		QList<Notify*> m_notifyList;	/*!< 当前通知列表 */
		int m_maxCount;		/*!< 最大显示通知数量 */
		int m_displayTime;	/*!< 单个通知最长显示时间 */
	};

} //namespace Base 