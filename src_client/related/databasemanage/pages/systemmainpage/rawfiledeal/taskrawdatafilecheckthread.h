/*!
 * @brief     任务原始数据文件集检测线程
 * @details	  用户遍历指定文件夹下所有数据文件信息
 * @author    yzg
 * @version   1.0
 * @date      2021.03.10 10:33:51
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QObject>

namespace Related {

	class TaskRawDataFileCheckThread : public QObject
	{
		Q_OBJECT

	public:
		TaskRawDataFileCheckThread(QObject *parent = nullptr);
		~TaskRawDataFileCheckThread();

		void setRawDataFileRootPath(QString path);
		bool checkRawDataDir();
		QStringList getFileLists();

	private:
		bool dealRawDataDir(QString path);

	private:
		QString  m_rootPath;					/*!< 根目录 */
		QStringList m_listFileNames;			/*!< 文件列表 */
	};

}//namespace Related 