#include "notifymanager.h"

#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QApplication>
#include <QTimer>
#include <QDebug>

#include "notify.h"

namespace Base {

#define NOTIFY_WIDTH 320
#define NOTIFY_HEIGHT 70

#define MARGIN_RIGHT 10
#define MARGIN_BOTTOM 10

#define NOTIFY_SPACE 20

	NotifyManager::NotifyManager(QObject *parent)
		: QObject(parent),
		m_maxCount(6),
		m_displayTime(4 * 1000), m_notifyAppeartion(Appear_Bottom)
	{
	}

	NotifyManager::~NotifyManager()
	{
	}

	void NotifyManager::information(QString content)
	{
		notify(QStringLiteral("提示"), ":/icon/resource/icon/alert_info.png", content);
	}

	void NotifyManager::warning(QString content)
	{
		notify(QStringLiteral("警告"), ":/icon/resource/icon/alert_warning.png", content);
	}

	void NotifyManager::notify(const QString &title, const QString &icon, const QString &content)
	{
		m_dataQueue.enqueue(NotifyData(icon, title, content));
		showNext();
	}

	void NotifyManager::setAppearPosition(NotifyApperPosition pos)
	{
		m_notifyAppeartion = pos;
	}

	void NotifyManager::setMaxCount(ushort count)
	{
		m_maxCount = count;
	}

	void NotifyManager::setDisplayTime(int ms)
	{
		m_displayTime = ms;
	}

	void NotifyManager::reArrange()
	{
		QRect screenRect = QApplication::desktop()->availableGeometry();

		int totalCount = m_notifyList.size();
		QPoint bottomRignt = screenRect.bottomRight();

		for (int i = 0; i < m_notifyList.size(); i++) {
			int offset = totalCount - i;

			QPoint pos;
			if (m_notifyAppeartion == Appear_Top) {
				pos = QPoint(screenRect.width() - NOTIFY_WIDTH - MARGIN_RIGHT, (NOTIFY_HEIGHT + NOTIFY_SPACE) * (offset)+MARGIN_BOTTOM + NOTIFY_SPACE);
			}
			else if (m_notifyAppeartion == Appear_Bottom) {
				pos = bottomRignt - QPoint(NOTIFY_WIDTH + MARGIN_RIGHT, (NOTIFY_HEIGHT + NOTIFY_SPACE) * (i + 1) - NOTIFY_SPACE + MARGIN_BOTTOM);
			}

			QPropertyAnimation *animation = new QPropertyAnimation(m_notifyList.at(i), "pos", this);
			animation->setStartValue(m_notifyList.at(i)->pos());
			animation->setEndValue(pos);
			animation->setDuration(300);
			animation->start();

			connect(animation, &QPropertyAnimation::finished, this, [animation, this]() {
				animation->deleteLater();
			});
		}
	}

	void NotifyManager::showNext()
	{
		if (m_notifyList.size() >= m_maxCount || m_dataQueue.isEmpty()) {
			return;
		}

		NotifyData data = m_dataQueue.dequeue();

		Notify * notify = new Notify(m_displayTime);
		notify->setIcon(data.icon);
		notify->setTitle(data.title);
		notify->setContent(data.body);

		notify->setFixedSize(NOTIFY_WIDTH, NOTIFY_HEIGHT);

		QDesktopWidget * desktop = QApplication::desktop();
		QRect desktopRect = desktop->availableGeometry();

		if (m_notifyAppeartion == Appear_Top) {
			reArrange();

			QPoint pos = QPoint(desktopRect.width() - notify->width() - MARGIN_RIGHT, MARGIN_BOTTOM + NOTIFY_SPACE);
			notify->move(pos);
		}
		else if (m_notifyAppeartion == Appear_Bottom) {
			QPoint bottomRignt = desktopRect.bottomRight();
			QPoint pos = bottomRignt - QPoint(notify->width() + MARGIN_RIGHT, (NOTIFY_HEIGHT + NOTIFY_SPACE) * (m_notifyList.size() + 1) - NOTIFY_SPACE + MARGIN_BOTTOM);

			notify->move(pos);
		}

		notify->showGriant();
		m_notifyList.append(notify);

		connect(notify, &Notify::disappeared, this, [notify, this]() {
			m_notifyList.removeAll(notify);
			if (m_notifyAppeartion == Appear_Bottom) {
				this->reArrange();
			}

			// 如果列表是满的，重排完成后显示
			if (m_notifyList.size() == this->m_maxCount - 1) {
				QTimer::singleShot(300, this, [this] {
					this->showNext();
				});
			}
			else {
				this->showNext();
			}
			notify->deleteLater();
		});
	}

} //namespace Base 