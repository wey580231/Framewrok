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
		QString m_originalDataFielPath;					/*!< 原始文件路径 */
		QString m_settleDataPath;						/*!< 处理完成数据路径 */
		Datastruct::FileType m_fileType;				/*!< 文件类型 */

		//RawDataFileProcessing * m_processing;							/*!< 原始dat数据处理 */
	};

}//namespace Related