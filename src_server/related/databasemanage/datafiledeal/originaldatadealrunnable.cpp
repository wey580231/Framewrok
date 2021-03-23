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
	 * @brief  设置文件路径
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
	 * @brief  用于处理原始数据的代理逻辑段
	 * @details 
	 */
	void OriginalDataDealRunnable::run()
	{
		//[] 处理Data数据
		if (m_fileType == Datastruct::File_Dat) {
			//m_processing = new RawDataFileProcessing();
			//m_package->dealFile(m_originalDataFielPath, m_settleDataPath);
		}
		else if(m_fileType == Datastruct::File_XML)
		{

		}
	}

}
