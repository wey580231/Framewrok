#include "statusbarcontainer.h"

#include <QHBoxLayout>
#include <QAction>

namespace Base {

	StatusBarContainer::StatusBarContainer(Id id, QObject *parent) :m_id(id), QObject(parent), m_container(nullptr)
	{
		m_container = new QWidget();
		m_container->setObjectName("statusbarwidget");
		m_container->setStyleSheet("QWidget#statusbarwidget{border-left:1px solid gray;}");

		m_itemLayout = new QHBoxLayout();
		m_itemLayout->setContentsMargins(3, 1, 1, 1);
		m_container->setLayout(m_itemLayout);
	}

	StatusBarContainer::~StatusBarContainer()
	{
		std::for_each(m_items.begin(), m_items.end(), [&](StatusBarItem & item) {
			delete item.m_widgetContainer;
		});
	}

	Id StatusBarContainer::id() const
	{
		return m_id;
	}

	QWidget *StatusBarContainer::container()
	{
		return m_container;
	}

	int StatusBarContainer::visibleItemSize()
	{
		return std::count_if(m_items.begin(), m_items.end(), [](StatusBarItem item) {return item.m_bVisible; });
	}

	void StatusBarContainer::setVisible(bool flag)
	{
		m_container->setVisible(flag);
	}

	StatusBarItem StatusBarContainer::item(Id id)
	{
		auto titer = std::find_if(m_items.begin(), m_items.end(), [&id](StatusBarItem item) {return item.m_id == id; });
		if (titer != m_items.end())
			return *titer;

		return StatusBarItem();
	}

	/*!
	* @brief 添加新item
	* @details 添加新的item至当前已有item的末尾，并且确保最后一个layoutitem为stretch
	* @param[in] item 待添加的item描述信息
	* @return nullptr:添加失败
	*         QAction *: 对应item的action控件，可添加至统一右键菜单
	*/
	QAction * StatusBarContainer::addItem(StatusBarItem item)
	{
		if (item.m_widgetContainer == nullptr)
			return nullptr;

		if (existedItem(item.m_id))
			return nullptr;

		if (m_itemLayout->count() == 0) {
			m_itemLayout->addWidget(item.m_widgetContainer);
			m_itemLayout->addStretch(1);
		}
		else {
			m_itemLayout->addWidget(item.m_widgetContainer);
		}
		return createItem(item);
	}

	/*!
	* @brief 在指定beforedId后插入item
	* @param[in] item 待添加的item描述信息
	* @return nullptr:添加失败
	*         QAction *: 对应item的action控件，可添加至统一右键菜单
	*/
	QAction * StatusBarContainer::insertItem(Id beforeId, StatusBarItem item)
	{
		if (item.m_widgetContainer == nullptr)
			return nullptr;

		if (existedItem(item.m_id))
			return nullptr;

		if (m_itemLayout->count() == 0) {
			return addItem(item);
		}
		else {
			auto titer = std::find_if(m_items.begin(), m_items.end(), [&beforeId](StatusBarItem & existedItem) {return beforeId == existedItem.m_id; });
			if (titer == m_items.end()) {
				return addItem(item);
			}
			else {
				for (int i = 0; i < m_itemLayout->count(); i++) {
					if (m_itemLayout->itemAt(i)->widget() == (*titer).m_widgetContainer) {
						m_itemLayout->insertWidget(i, item.m_widgetContainer);
						return createItem(item);
					}
				}
			}
		}
	}

	/*!
	* @brief 添加一个弹簧
	* @details 若添加的位置之前已存在一个弹簧，则忽略本次添加
	*/
	void StatusBarContainer::addStretch(int stretch)
	{
		if (m_itemLayout->count() == 0) {
			m_itemLayout->addStretch(stretch);
		}
		else {
			if (m_itemLayout->itemAt(m_itemLayout->count() - 1)->spacerItem() == nullptr)
				m_itemLayout->addStretch(stretch);
		}
	}

	/*!
	* @brief 设置容器固定宽度
	* @param[in] width 宽度值
	*/
	void StatusBarContainer::setFixedWidth(int width)
	{
		if (width <= 0)
			return;

		m_container->setFixedWidth(width);
	}

	QAction *StatusBarContainer::createItem(StatusBarItem item)
	{
		QAction * t_action = new QAction(item.m_text);
		t_action->setCheckable(true);
		connect(t_action, SIGNAL(toggled(bool)), this, SLOT(actionChecked(bool)));

		t_action->setChecked(item.m_bVisible);
		item.m_action = t_action;
		item.m_widgetContainer->setVisible(item.m_bVisible);
		item.m_widgetContainer->setParent(m_container);

		m_items.append(item);

		return t_action;
	}

	void StatusBarContainer::actionChecked(bool flag)
	{
		QAction * tSender = dynamic_cast<QAction *>(QObject::sender());
		if (tSender) {
			auto titer = std::find_if(m_items.begin(), m_items.end(), [&tSender](StatusBarItem & item) {return item.m_action == tSender; });
			if (titer != m_items.end()) {
				(*titer).m_bVisible = flag;
				(*titer).m_widgetContainer->setVisible(flag);
				emit actionCheckstateChanged(this);
			}
		}
	}

	/*!
	* @brief 判断是否存在对应id的项
	* @param[in] id 待测试id项
	* @return true:存在；false:不存在
	*/
	bool StatusBarContainer::existedItem(Id id)
	{
		auto tFindIndex = std::find_if(m_items.begin(), m_items.end(), [&id](StatusBarItem &existedItem) {return existedItem.m_id == id; });
		return tFindIndex != m_items.end();
	}

} //namespace Base