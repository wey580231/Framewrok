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

	// �����ϴ�
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
	 * @brief    �����ļ��ϴ����
	 * @details 
	 */
	void UploadingDataFile::processTaskDataFileCeateResponse(const Datastruct::TaskDataFileCreateResponse & response)
	{
		if (response.m_md5 == m_fileDescription.m_md5 && response.m_taskId == m_fileDescription.m_taskId) {
			
			if (response.m_result) {
				m_b_sendFile = true;

				m_fileId = response.m_id;
				m_currentLength = response.m_result;
				
				//[] ���ļ� 
				bool b_open = openFile();
				if (!b_open) {
					emit signalSendFileStataus(false);
					return ;
				}
				//[] ����֮ǰ�ϴ����ļ�
				if (response.m_lastLen > 0) {
					m_file->skip(response.m_lastLen);
				}
				//[] �����ļ�
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
	 * @brief �����ļ��Ļ���������Ϣ
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
		Global::G_Notify->information(QStringLiteral("�ļ��������!"));
		if (m_file != nullptr) {
			m_file->close();
		}
 		delete m_file;
		m_file = nullptr;
		emit signalSendFileStataus(true);
	}

	/*!
	 * @brief    �����ļ�
	 * @details  ���ļ��ж�ȡָ����С�����䷢����������
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
				parameter.m_totalLength = m_file->size();
				parameter.m_currentLength = m_currentLength;

				FileNetConnector::instance()->write(parameter, data);

				emit signalSendFileProgressStataus(m_fileDescription.m_size, m_currentLength);

				if (m_file->atEnd()) {
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