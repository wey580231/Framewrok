#include "originaldatadealrunnable.h"

#include <QDebug>

namespace Related {

	OriginalDataDealRunnable::OriginalDataDealRunnable()
	{
	
	}

	OriginalDataDealRunnable::~OriginalDataDealRunnable()
	{
	}

	/*!
	 * @brief  �����ļ�·��
	 */
	void OriginalDataDealRunnable::setDataFilePath(QString path)
	{
		m_originalDataFielPath = path;
	}

	void OriginalDataDealRunnable::setResolveDataFilePath(QString path)
	{
		m_settleDataPath = path;
	}

	void OriginalDataDealRunnable::setFileType(Datastruct::FileType type)
	{
		m_fileType = type;
	}

	/*!
	 * @brief  ���ڴ���ԭʼ���ݵĴ����߼���
	 * @details 
	 */
	void OriginalDataDealRunnable::run()
	{
		//[] ����Data����
		if (m_fileType == Datastruct::File_Dat) {
			//m_processing = new RawDataFileProcessing();
			//m_package->dealFile(m_originalDataFielPath, m_settleDataPath);
		}
		else if(m_fileType == Datastruct::File_XML)
		{

		}
	}

}
