#include "rtableview.h"

#include <QHeaderView>
#include <QDebug>
#include <QAction>

#include "rtablemodel.h"
#include "tablecolumneditwindow.h"
#include "../rmessagebox.h"

namespace Base {

	RTableView::RTableView(QWidget *parent)
		: QTableView(parent), m_headMenu(nullptr), m_ascending(false), m_columResizeByUser(true)
	{
		verticalHeader()->setVisible(false);
		verticalHeader()->setDefaultSectionSize(35);

		horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
		horizontalHeader()->setStretchLastSection(true);

		connect(horizontalHeader(), SIGNAL(sectionResized(int, int, int)),this,SLOT(respColumnResize(int, int, int)));

		setTableFeature(T_ColumnEdit, true);
		setTableFeature(T_HeadSorting, true);
		setTableFeature(T_AlternatingRowColors, true);
	}

	RTableView::~RTableView()
	{

	}

	void RTableView::setModel(RTableModel *model)
	{
		if (model) {
			model->updateColumnItems(m_columns);
			QTableView::setModel(model);
		}
	}

	bool RTableView::addColumnItem(ColumnItem item)
	{
		auto result = std::find_if(m_columns.begin(), m_columns.end(), [&item](ColumnItem & titem) {
			return item.m_id == titem.m_id || item.m_name == titem.m_name;
		});

		if (result != m_columns.end())
			return false;

		m_columns.append(item);

		RTableModel * tmodel = dynamic_cast<RTableModel *>(model());
		if (tmodel) {
			tmodel->updateColumnItems(m_columns);
		}

		if (item.m_columnWidth > 0) {
			int visibleCount = std::count_if(m_columns.begin(), m_columns.end(), [](ColumnItem & titem) {return titem.m_visible;});
			horizontalHeader()->resizeSection(visibleCount - 1, item.m_columnWidth);
		}

		return true;
	}

	void RTableView::setTableFeature(TableFeature feature, bool enabled)
	{
		if (enabled)
			m_tableFeatures |= feature;
		else
			m_tableFeatures &= ~feature;

		udateFeature();
	}

	void RTableView::respCustomContextMenu(const QPoint & point)
	{
		if (m_headMenu == nullptr) {
			m_headMenu = new QMenu();
		}

		m_headMenu->clear();

		QAction * controlColumnVisible = new QAction(QIcon(QStringLiteral(":/QYBlue/resource/qyblue/编辑.png")), QStringLiteral("编辑列"));
		connect(controlColumnVisible, SIGNAL(triggered(bool)), this, SLOT(showColumnVisibleEditWindow(bool)));

		m_headMenu->addSeparator();

		std::for_each(m_columns.begin(), m_columns.end(), [&](const ColumnItem & item) {
			QAction * action = new QAction();
			action->setText(item.m_name);
			action->setCheckable(true);
			action->setChecked(item.m_visible);
			action->setData(item.m_id);

			connect(action, SIGNAL(triggered(bool)), this, SLOT(updateModelColumnVisible(bool)));

			m_headMenu->addAction(action);
		});

		m_headMenu->addAction(controlColumnVisible);

		m_headMenu->exec(QCursor::pos());
	}

	void RTableView::showColumnVisibleEditWindow(bool flag)
	{
		RTableModel * customModel = dynamic_cast<RTableModel*>(model());
		if (customModel) {
			TableColumnEditWindow editWindow(this);
			editWindow.setItems(m_columns);

			if(editWindow.exec() == QDialog::Accepted){
				m_columns = editWindow.getItems();
				customModel->updateColumnItems(m_columns);
				resizeColumnWidth();
			}
		}
	}

	void RTableView::updateModelColumnVisible(bool flag)
	{
		QAction * action = dynamic_cast<QAction *>(QObject::sender());
		if (action) {
			RTableModel * customModel = dynamic_cast<RTableModel*>(model());
			if (customModel) {
				if (customModel->visibleColumnCount() == 1 && flag == false) {
					RMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("至少勾选一列!"), RMessageBox::Yes);
					return;
				}

				for (int i = 0; i < m_columns.size(); i++) {
					if (m_columns.at(i).m_id == action->data().toInt()) {
						m_columns[i].m_visible = flag;
						break;
					}
				}

				//NOTE 避免在此从隐藏->显示过程中，多次触发
				m_columResizeByUser = false;
				customModel->setColumVisible(action->data().toInt(), flag);
				customModel->refresh();
				m_columResizeByUser = true;

				resizeColumnWidth();
			}
		}
	}

	void RTableView::respSectionClicked(int column)
	{
		RTableModel * tmodel = dynamic_cast<RTableModel *>(model());
		if (!tmodel)
			return;

		if (m_lastClickedColumn != column) {
			m_ascending = true;
		}

		Qt::SortOrder st = m_ascending ? Qt::AscendingOrder : Qt::DescendingOrder;
		tmodel->sortByColumn(column, st);
		tmodel->refresh();

		horizontalHeader()->setSortIndicator(column, st);

		m_ascending = !m_ascending;
		m_lastClickedColumn = column;
	}

	void RTableView::respColumnResize(int logicalIndex, int oldSize, int newSize)
	{
		if (!m_columResizeByUser)
			return;

		int loop = 0;
		for (int i = 0; i < m_columns.size(); i++) {
			if (m_columns[i].m_visible) {
				m_columns[i].m_columnWidth = columnWidth(loop++);
			}
		}
	}

	void RTableView::udateFeature()
	{
		disconnect(horizontalHeader(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(respCustomContextMenu(const QPoint &)));
		disconnect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(respSectionClicked(int)));

		if (m_tableFeatures.testFlag(T_ColumnEdit)) {
			if (horizontalHeader()) {
				horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
				connect(horizontalHeader(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(respCustomContextMenu(const QPoint &)));
			}
		}
		else {
			if (horizontalHeader()) {
				horizontalHeader()->setContextMenuPolicy(Qt::DefaultContextMenu);
			}
		}

		setSortingEnabled(m_tableFeatures.testFlag(T_HeadSorting));

		if (m_tableFeatures.testFlag(T_HeadSorting)) {
			connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(respSectionClicked(int)));
			horizontalHeader()->setSortIndicatorShown(true);
		}
		else {
			horizontalHeader()->setSortIndicatorShown(false);
		}

		setAlternatingRowColors(m_tableFeatures.testFlag(T_AlternatingRowColors));
	}

	void RTableView::resizeColumnWidth()
	{
		m_columResizeByUser = false;
		int loop = 0;
		for (int i = 0; i < m_columns.size(); i++) {
			if (m_columns.at(i).m_visible)
				setColumnWidth(loop++, m_columns.at(i).m_columnWidth);
		}
		m_columResizeByUser = true;
	}

} //namespace Base 