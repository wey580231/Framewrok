/*!
 * @brief     Ŀ������  
 * @details   �ṩ
 * @author    yzg
 * @version   1.0
 * @date      2021.01.20 10:43:37
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QWidget>
#include <qstackedwidget.h>

#include <base\selfwidget\rtabwidget.h>

#include "../abstractpage.h"

namespace Related {

	class TargetOverViewPage;
	class TargetDetailPage;

	class FrequencyDomainAnalyse;
	class TimeDomainAnalyse;
	class DynamicGraphAnalysis;
	class DemonGraph;
	class LofarGraph;
	class SituationAnalysis;

	class TargetDatabaseWidget : public AbstractPage
	{
		Q_OBJECT

	public:
		TargetDatabaseWidget(QWidget *parent = nullptr);
		~TargetDatabaseWidget();

		PageType getPageType() const;

		void prepareBringToTop();

	private slots:
		void slotSelecteTargetDataInfo(QString indexId);

	private:
		void init();

	private:
		Base::RTabWidget * m_tabWidget;							/*!<  */
		TargetOverViewPage * m_targetOverViewPage;				/*!< Ŀ���������� */
		TargetDetailPage * m_targetDetailPage;					/*!< Ŀ������ҳ�� */

		TimeDomainAnalyse * m_timeAnalyseWidget;			/*!< ʱ�����ҳ�� */
		DynamicGraphAnalysis * m_audioDataWidget;			/*!< Ƶ�������� */
		DemonGraph * m_demonAnalysis;						/*!< demon�������� */
		LofarGraph * m_lofarAnalysis;						/*!< lfoar�������� */
		SituationAnalysis * m_situationAnalysis;			/*!< ̬�Ʒ���ҳ�� */

	};

}//namespace Related 