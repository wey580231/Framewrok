/*!
 * @brief     页面切换工具栏
 * @details	  提供通用的的页面切换组件，包括切换首页、上一页、下一页、尾页
 * @author    wey
 * @version   1.0
 * @date      21.01.14 14:43:19
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <base\selfwidget\iconbutton.h>

class QComboBox;
class QLineEdit;

namespace Related {

	class PageSwitchBar : public QWidget
	{
		Q_OBJECT

	public:
		PageSwitchBar(QWidget *parent = nullptr);
		~PageSwitchBar();

		void setDataSize(int dataSize);

	signals:
		void perPageNumsChanged(int perPageNum);
		void switchPage(int curPage);

	private slots:
		void respPageCountChanged(int pageIndex);
		void buttPressed();

	private:
		void init();

		void reCalc();
		void updateButtState();
		void updatePageIndicate();

	private:
		enum ButtType {
			FIRST_PAGE,
			PRI_PAGE,
			NEXT_PAGE,
			LAST_PAGE
		};

	private:
		Base::RIconButton * m_firstButton;
		Base::RIconButton * m_prePageButton;
		Base::RIconButton * m_nextPageButton;
		Base::RIconButton * m_lastButton;

		QComboBox * m_pageItemCountBox;
		QLineEdit * m_pageNumLabel;

		int m_dataSize;				/*!< 数据条数 */
		int m_pageCount;			/*!< 页面总数 */
		int m_currPage;				/*!< 当前页面索引 */
		int m_perPageItemCount;		/*!< 当前页面数据条数 */		
	};

} //namespace Related 
