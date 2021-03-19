#include "uploadingdatafile.h"

#include <QDebug>


#include "../../net/datanetconnector.h"
#include "../../net/signaldispatch.h"
#include "../../net/filenetconnector.h"
#include "../../utils/util.h"
#include "../../global.h"

namespace Related {

	UploadingDataFile::UploadingDataFile(QObject *parent)
		: QObject(parent),  m_currentLength(0), m_file(nullptr), m_b_sendFile(false), m_pause(false)
	{
		initConnect();
	}

	UploadingDataFile::~UploadingDataFile()
	{

	}

	void UploadingDataFile::setFileDescription(FileDescriptionData description)
	{
		m_fileDescription = description;
	}

	// 启动上传
	void UploadingDataFile::startUploading()
	{
		Datastruct::TaskDataFileCreateRequest request;
		request.m_taskId = m_fileDescription.m_taskId;
		request.m_name = m_fileDescription.m_name;
		request.m_suffix = m_fileDescription.m_suffix;
		request.m_timeStamp = m_fileDescription.m_fileTimeStamp;
		request.m_size = m_fileDescription.m_size;
		request.m_md5 = m_fileDescription.m_md5;

		sendCreateTaskDataFileInfo(request);
	}

	void UploadingDataFile::pauseSendFile()
	{
		m_pause = true;
	}

	void UploadingDataFile::continueSendFile()
	{
	}

	/*!
	 * @brief    接受文件上传结果
	 * @details 
	 */
	void UploadingDataFile::processTaskDataFileCeateResponse(const Datastruct::TaskDataFileCreateResponse & response)
	{
		if (response.m_md5 == m_fileDescription.m_md5 && response.m_taskId == m_fileDescription.m_taskId) {
			
			if (response.m_result) {
				m_b_sendFile = true;

				m_fileId = response.m_id;
				m_currentLength = response.m_result;
				
				//[] 打开文件 
				bool b_open = openFile();
				if (!b_open) {
					emit signalSendFileStataus(false);
					return ;
				}
				//[] 处理之前上传的文件
				if (response.m_lastLen > 0) {
					m_file->skip(response.m_lastLen);
				}
				//[] 发送文件
				dealFile();
			}
		}
	}

	void UploadingDataFile::slotSendFileStatus(Datastruct::ConnectionType type)
	{
		if (type == Datastruct::File_Connection) {
			if (m_b_sendFile == true) {
				dealFile();
			}
		}
	}

	void UploadingDataFile::initConnect()
	{
		connect(SignalDispatch::instance(), SIGNAL(respTaskDataFileCreateResponse(const Datastruct::TaskDataFileCreateResponse &)),
			this, SLOT(processTaskDataFileCeateResponse(const Datastruct::TaskDataFileCreateResponse &)));

		connect(FileNetConnector::instance(), SIGNAL(writeDataResult(Datastruct::ConnectionType)), 
			this, SLOT(slotSendFileStatus(Datastruct::ConnectionType)));
	}

	/*!
	 * @brief 发送文件的基本描述信息
	 */
	void UploadingDataFile::sendCreateTaskDataFileInfo(Datastruct::TaskDataFileCreateRequest request)
	{
		request.m_taskId = m_fileDescription.m_taskId;
		DataNetConnector::instance()->write(request);
	}


	bool UploadingDataFile::openFile()
	{
		m_file = new QFile(m_fileDescription.m_filePath);
		if (m_file->exists() && m_file->open(QIODevice::ReadOnly)) {
			return true;
		}
		return false;
	}

	void UploadingDataFile::closeFile()
	{
		Global::G_Notify->information(QStringLiteral("文件传输完成!"));
		if (m_file != nullptr) {
			m_file->close();
		}
 		delete m_file;
		m_file = nullptr;
		emit signalSendFileStataus(true);
	}

	/*!
	 * @brief    处理文件
	 * @details  从文件中读取指定大小并将其发送至服务器
	 */
	void UploadingDataFile::dealFile()
	{	
		if (m_file != nullptr) {
			if (!m_file->atEnd()) {

				QByteArray data = m_file->read(SEND_FILE_LENGTH);
				m_currentLength += data.length();
				// 
				Datastruct::FileInfoParameter  parameter;
				memcpy(parameter.m_fileId, m_fileId.toLocal8Bit().data(), m_fileId.toLocal8Bit().size());
				memcpy(parameter.m_md5, m_fileDescription.m_md5.toLocal8Bit().data(), m_fileDescription.m_md5.toLocal8Bit().size());
				parameter.m_totalLength = m_file->size();
				parameter.m_currentLength = data.size();

				FileNetConnector::instance()->write(parameter, data);

				emit signalSendFileProgressStataus(m_fileDescription.m_size, m_currentLength);

				bool t_b_fileEnd = m_file->atEnd();

				if (t_b_fileEnd) {
					closeFile();
				}
			}
			else
			{
				closeFile();
			}
		}
	}

}//namespace Related