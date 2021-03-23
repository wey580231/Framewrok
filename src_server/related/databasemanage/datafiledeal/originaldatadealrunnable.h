/*!
 * @brief     
 * @details 
 * @author    yzg
 * @version   1.0
 * @date      2021.03.19 14:47:59
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QObject>
#include <QRunnable>

#include <base/util/rutil.h>
#include <commondefines/protocol.h>

//#include "rawdatafileprocessing.h"

namespace Related {

	class OriginalDataDealRunnable : public QObject, public QRunnable
	{
		Q_OBJECT

	public:
		OriginalDataDealRunnable();
		~OriginalDataDealRunnable();

		void setDataFilePath(QString  path);
		void setResolveDataFilePath(QString path);
		void setFileType(Datastruct::FileType type);

		void run();

	private:
		QString m_originalDataFielPath;					/*!< ԭʼ�ļ�·�� */
		QString m_settleDataPath;						/*!< �����������·�� */
		Datastruct::FileType m_fileType;				/*!< �ļ����� */

		//RawDataFileProcessing * m_processing;							/*!< ԭʼdat���ݴ��� */
	};

}//namespace Related