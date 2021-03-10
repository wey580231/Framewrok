/*!
 * @brief     任务原始数据文件处理线程
 * @details	  根据任务，将原始数据文件进行分块，分块发送数据
 * @author    yzg
 * @version   1.0
 * @date      2021.03.10 09:23:35
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QObject>

#include <core/network/rtask.h>

namespace Related {

	class TaskRawDataFileDealThread : public Core::RTask
	{
		Q_OBJECT

	public:
		TaskRawDataFileDealThread(QObject *parent = nullptr);
		~TaskRawDataFileDealThread();

		void startMe();
		void stopMe();

	protected:
		void run();

	private:
		void dealFile();

	};

}//namespace Related 