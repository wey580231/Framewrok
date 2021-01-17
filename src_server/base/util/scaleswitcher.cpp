#include "scaleswitcher.h"

#include <QDebug>
#include <math.h>

#ifdef Q_OS_WIN
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#endif

namespace Base {

	ScaleSwitcher::ScaleSwitcher()
	{

	}

	/*!
	 * @brief 将16或10进制字符串转换成整形
	 * @param[in] data 形如"0xAB"、"AB","12"等字符串
	 * @param[in/out] error 是否转换错误，转换出错时，置为true
	 * @param[in] base 当前数据的基数
	 */
	int ScaleSwitcher::toInt(QString data, bool &error, ScaleSwitcher::SwitchBase base)
	{
		long long result = 0;
		error = false;

		QString lowData = data.toLower();
		if (lowData.startsWith("0x"))
			lowData = lowData.right(lowData.length() - 2);

		QByteArray array = lowData.toLower().toLocal8Bit().data();
		for (int i = 0; i < array.length(); i++) {
			int exp = array.length() - i - 1;
			if (array.at(i) >= '0' && array.at(i) <= '9') {
				if (base == DEC) {
					error = true;
					break;
				}

				switch (base) {
				case DEC:result += (array.at(i) - 48)*pow(10, exp); break;
				case HEX:result += (array.at(i) - 48)*pow(16, exp); break;
				default:break;
				}
			}
			else if (array.at(i) >= 'a' && array.at(i) <= 'f') {
				switch (base) {
				case HEX:result += (array.at(i) - 87)*pow(16, exp); break;
				default:break;
				}
			}
			else {
				error = true;
				break;
			}
		}

		if (error) {
			return 0;
		}

		return result;
	}

	/*!
	 * @brief 将16进制转换成10进制
	 * @details 若转换成功则返回转换的信息，否则返回0
	 * @param[in]  number 待转换的16进制数
	 * @return 返回转换结果
	 */
	unsigned short ScaleSwitcher::fromHexToDec(QString &number)
	{
		bool ok = false;
		unsigned short result = static_cast<unsigned short>(number.toInt(&ok, 16));
		if (ok)
			return result;

		return 0;
	}

	unsigned short ScaleSwitcher::htons(unsigned short number)
	{
		return ::htons(number);
	}

	unsigned short ScaleSwitcher::ntohs(unsigned short number)
	{
		return ::ntohs(number);
	}

	unsigned long ScaleSwitcher::htonl(unsigned long number)
	{
		return ::htonl(number);
	}

	unsigned long ScaleSwitcher::ntohl(unsigned long number)
	{
		return ::ntohl(number);
	}

	/*!
	 * @brief 将十进制表示形式转换成度分秒
	 * @attention 格式为" XX°XX′XX″ "
	 * @param[in] lonlat 十进制坐标位置
	 * @return 度分秒
	 */
	QString ScaleSwitcher::switchLonLatToDegree(double lonlat)
	{
		int i = 0;
		QStringList result;
		while (i++ <= 2) {
			double tmp = floor(lonlat);
			result.append(QString::number(tmp, 'f', 0));
			lonlat = (lonlat - floor(lonlat)) * 60;
		}

		return QString(QStringLiteral("%1°%2′%3″")).arg(result.at(0)).arg(result.at(1)).arg(result.at(2));
	}

	/*!
	 * @brief 将度分秒转换成十进制
	 * @attention 此处只能类似于适应" XX°XX′XX″ "格式的字符串,其它的会返回0
	 * @param[in] degree 度分秒格式的地理信息
	 * @return 十进制位置信息
	 */
	double ScaleSwitcher::switchDegreeToLonLat(QString degree)
	{
		degree = degree.trimmed();
		QRegExp exp("(\\d+)");

		double value = 0;
		int weight = 1;
		int pos = 0;
		while ((pos = exp.indexIn(degree, pos)) != -1) {
			value += exp.cap(1).toDouble() / weight;
			weight *= 60;

			pos += exp.matchedLength();
		}

		return value;
	}

	/*!
	 * @brief 获取经度的英文简写
	 * @details 默认东经和北纬值大于0,西经和南纬值小于0
	 * @param[in] lon
	 * @return 简写值
	 */
	QString ScaleSwitcher::getLonAbbreviate(double lon)
	{
		return lon > 0 ? ("E") : ("W");
	}

	QString ScaleSwitcher::getLatAbbreviate(double lat)
	{
		return lat > 0 ? ("N") : ("S");
	}

	/*!
	 * @brief 线性插值
	 * @param[in] keyA 下界
	 * @param[in] valueA
	 * @param[in] keyB 上界
	 * @param[in] valueB
	 * @param[in] inputKey 待插入key
	 * @return 线性计算后value
	 */
	double ScaleSwitcher::linearInterpolation(double keyA, double valueA, double keyB, double valueB, double inputKey)
	{
		if (keyA != keyB) {
			double k = (valueB - valueA) / (keyB - keyA);
			double val = (inputKey - keyA) * k + valueA;
			return val;
		}
		return 0;
	}

	/*!
	 * @brief 十六进制字符转整数
	 * @details 将形如：A、B、C等字符转换成整形
	 * @param 待转换的十六进制字符
	 * @return 转换后的整数值
	 */
	int ScaleSwitcher::hexChar2Number(char ch)
	{
		if (ch >= '0' && ch <= '9')
			return ch - 0x30;
		if (ch >= 'A' && ch <= 'F')
			return ch - 0x37;
		return 0;
	}

	char ScaleSwitcher::toHexUpper(uint value)
	{
		static char buff[] = "0123456789ABCDEF";
		return buff[value & 0xF];
	}

	char ScaleSwitcher::toHexLower(uint value)
	{
		static char buff[] = "0123456789abcdef";
		return buff[value & 0xF];
	}

} //namespace Base