/*!
 *  @brief     状态栏一组控件显示容器
 *  @details   每个容器有唯一的id，容器内部可放置多个控件。多个容器之间通过qsplitter分割;
 *             1.容器在默认添加一个控件时，会自动在其后加入一个spacer(弹簧)，确保控件在最左显示；
 *             2.容器提供了和qlayout相似的功能，可以添加item(addItem)也可以添加spacer(addStretch)
 *  @author    wey
 *  @version   1.0
 *  @date      2019.08.15
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef STATUSBARCONTAINER_H
#define STATUSBARCONTAINER_H

#include <QObject>
#include <QWidget>

#include "../base_global.h"
#include "id.h"

class QHBoxLayout;

namespace Base {

	struct StatusBarItem
	{
		StatusBarItem() :m_bVisible(true), m_action(nullptr), m_widgetContainer(nullptr) {}
		Base::Id m_id;                        /*!< id */
		QString m_text;                 /*!< 右键菜单显示信息 */
		bool m_bVisible;                /*!< 是否可见 */
		QAction * m_action;             /*!< 右键弹出菜单 */
		QWidget * m_widgetContainer;    /*!< 页面窗口 */
	};

	struct StatusBarContainerItem {
		Id m_id;
		QList<StatusBarItem> m_items;
	};

	class BASESHARED_EXPORT StatusBarContainer : public QObject
	{
		Q_OBJECT
	public:
		explicit StatusBarContainer(Id id, QObject * parent = 0);
		~StatusBarContainer();

		Id id() const;
		QWidget * container();

		int size() { return m_items.size(); }
		int visibleItemSize();

		void setVisible(bool flag);

		StatusBarItem item(Id id);
		QAction * addItem(StatusBarItem item);
		QAction *insertItem(Id beforeId, StatusBarItem item);
		QList<StatusBarItem> items() const { return m_items; }

		void addStretch(int stretch = 1);

		void setFixedWidth(int width);

	signals:
		void actionCheckstateChanged(StatusBarContainer *);

	private slots:
		void actionChecked(bool flag);
		\
	private:
		QAction * createItem(StatusBarItem item);
		bool existedItem(Id id);

	private:
		Id m_id;
		QWidget * m_container;
		QList<StatusBarItem> m_items;
		QHBoxLayout * m_itemLayout;
	};

} //namespace Base

#endif // STATUSBARCONTAINER_H
