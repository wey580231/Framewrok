/*!
 *  @brief     通用工具类
 *  @details   定义通用的函数工具
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.11
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef RUTIL_H
#define RUTIL_H

#include <QString>
#include <QSettings>

#include "../base_global.h"

namespace Base {

	class BASESHARED_EXPORT RUtil
	{
	public:
		RUtil();

		/*!< 文件操作 */
		static void showInExplorer(QString &pathIn);

		static bool createDir(QString dirPath);
		static bool isFileExist(QString filePath);
		static bool isDir(QString dirPath);

		static QString UUID();

		static QString MD5(QString text);
		static QString MD5File(QString fileName);

		/*!< 时间戳、时间 */
		static QString getTimeStamp(QString format = "yyyy-MM-dd hh:mm:ss:zzz");
		static QString getDate(QString format = "yyyy-MM-dd");
		static qint64 timeStamp();

		static qint64 currentMSecsSinceEpoch();
		static qint64 currentSecsSinceEpoch();

		static QDateTime addMSecsToEpoch(qint64 mSeonds);
		static QDateTime addSecsToEpoch(qint64 seonds);

		/*!< 将整形/长整形转换成ip地址、mac地址 */
		static QString swithIntToIp(int value, bool reverse = true);
		static QString swithLongToMac(qulonglong value, bool reverse = true);
		static bool validateIpFormat(QString dest);

		/*!< 全局INI访问 */
		static void setGlobalSettings(QSettings * settings);
		static QSettings * globalSettings();
		static QVariant getGlobalValue(const QString & group, const QString &key, const QVariant &defaultValue);
		static void setGlobalValue(const QString & group, const QString &key, const QVariant &defaultValue);

		static QString replaceLongTextWidthElide(const QFont & font, const QString & origin, const int maxLen);

		/*!< bytes转GB等 */
		static QString switchBytesUnit(quint64 bytes);

		/*!< 屏幕信息 */
		static QSize screenSize(int num = -1);
		static QRect screenGeometry();
		static QRect centerScreen(QSize & widgetSize);	

		/*!< 经纬度转换 */
		static QString switchLonLatToDegree(double lonlatVal);

		static QImage convertToGray(const QImage &);

		/*!< 位置关系 */
		static QPoint placeAInBCenter(QSize a,QSize b);

	private:
		static QSettings * gSettings;   //全局配置文件  只设置一次
	};

} //namespace Base

#endif // RUTIL_H
