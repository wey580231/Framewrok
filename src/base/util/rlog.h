/*!
 *  @brief     日志记录
 *  @details   可记录不同级别的信息，包括出错的文件、行号、错误信息等。
 *  @file      rlog.h
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.13
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note 20180308:wey:修复多线程访问，因未加互斥锁产生的奔溃问题;
 *        20181207:wey:精简文件名、函数名显示
 */
#ifndef RLOG_H
#define RLOG_H

#include <QFile>

#include "../base_global.h"

class BASESHARED_EXPORT RLog : public QObject
{
	Q_OBJECT
public:

	/*!
	 *  @brief 日志等级
	 */
	enum RLOG_LEVEL
	{
		RINFO,               /*!<    提示  */
		RWARNING,            /*!<    警告  */
		RERROR               /*!<    提示  */
	};
	Q_FLAG(RLOG_LEVEL)

		struct LogConfig
	{
		bool isRecord2File;     /*!< 是否记录到文件 */
		int level;              /*!< 记录日志等级，大于等于此level的日志将被记录 */
	};

	RLog();

	void setLogLevel(const RLog::RLOG_LEVEL & level);
	RLog::RLOG_LEVEL getLogLevel() { return logLevel; }

	static bool init(RLog::LogConfig & logConfig);

	static void log(RLOG_LEVEL nLevel, const char * fileDesc, const char * functionDesc, int lineNum, const char* data, ...);

private:
	static QString getLeveDesc(RLOG_LEVEL level);

private:
	static bool isRecord2File;
	static bool isFileReady;
	static RLOG_LEVEL logLevel;
};

#ifdef Q_OS_WIN
#define FILE_SEPARATOR '\\'
#else
#define FILE_SEPARATOR '/'
#endif

#define FUNC_SEPARATOR '::'

#ifndef QT_NO_DEBUG
#define __FILENAME__ (strrchr(__FILE__, FILE_SEPARATOR) ? (strrchr(__FILE__, FILE_SEPARATOR) + 1):__FILE__)
#define __FUNNAME__ (strrchr(__FUNCTION__,FUNC_SEPARATOR)?(strrchr(__FUNCTION__, FUNC_SEPARATOR) + 1):__FUNCTION__)
#else
#define __FILENAME__ NULL
#define __FUNNAME__ NULL
#endif

#define RLOG_INFO(...)       RLog::log(RLog::RINFO,__FILENAME__,__FUNNAME__,__LINE__, __VA_ARGS__)
#define RLOG_WARNING(...)    RLog::log(RLog::RWARNING, __FILENAME__,__FUNNAME__, __LINE__,__VA_ARGS__)
#define RLOG_ERROR(...)      RLog::log(RLog::RERROR,__FILENAME__,__FUNNAME__,__LINE__,__VA_ARGS__)

#endif // RLOG_H
