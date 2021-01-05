/*!
 *  @brief     进制转换工具
 *  @details   满足将十进制转换为十六进制
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.01
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef SCALESWITCHER_H
#define SCALESWITCHER_H

#include "../base_global.h"

#include <QString>

namespace Base {

	class BASESHARED_EXPORT ScaleSwitcher
	{
	public:
		ScaleSwitcher();

		/*!
		 *  @brief 转换基数
		 */
		enum SwitchBase {
			HEX,
			DEC
		};

		/*!< 10/16进制字符串转10进制 */
		static int toInt(QString data, bool &error, SwitchBase base = DEC);
		static int hexChar2Number(char ch);

		template<class T>
		static QString switchOctToBinary(T src, int bitNum, QString splitCode = ";", bool reversed = false);

		/*!< 10进制转16进制字符 */
		static char toHexUpper(uint value);
		static char toHexLower(uint value);

		/*!< 16进制字符串与10进制数字转换 */
		static unsigned short fromHexToDec(QString & number);

		template<class T>
		static QString fromDecToHex(T number);


		/*!< 主机字节序与网络字节序互转 */
		static unsigned short htons(unsigned short number);
		static unsigned short ntohs(unsigned short number);
		static unsigned long htonl(unsigned long number);
		static unsigned long ntohl(unsigned long number);

		/*!< 将某值转换至[0,referVal)之间 */
		template<class T>
		static T switchValInRange(T value, T referVal);

		/*!< 经纬度转换 */
		static QString switchLonLatToDegree(double lonlat);
		static double switchDegreeToLonLat(QString degree);

		/*!< 获取经纬度的英文简写 */
		static QString getLonAbbreviate(double lon);
		static QString getLatAbbreviate(double lat);

		/*!< 线性插值 */
		static double linearInterpolation(double keyA, double valueA, double keyB, double valueB, double inputKey);
	};

	/*!
	 * @brief 将10进制转换成16进制
	 * @warning 转换后的结果不包含0x标志，即10进制9779,对应的16进制为0x2633，此处只返回2633。 \n
	 * @param[in] number 待转换的10进制数
	 * @return 返回转换结果
	 */
	template<class T>
	QString ScaleSwitcher::fromDecToHex(T number)
	{
		static const char HEX[] = "0123456789ABCDEF";
		QString str;
		int t_i_loopTime = sizeof(number);
		for (int i = 0; i < t_i_loopTime; i++)
			str.insert(0, HEX[number >> (i * 4) & 0x0F]);
		return str;
	}

	/*!
	* @brief 将值转换至0-X之间，并且保留小数显示
	* @details 若希望将值转换至0-360，那么value填写待转换值，referVal填写360
	* @param[in] value 待转换值
	* @param[in] referVal 转换时参考值
	*/
	template<class T>
	T ScaleSwitcher::switchValInRange(T value, T referVal)
	{
		if (value > referVal) {
			while (value > referVal) {
				value -= referVal;
			}
		}
		else if (value < 0) {
			while (value < 0) {
				value += referVal;
			}
		}
		return value;
	}


	/*!
	* @brief 将整形转换成二进制，并按照指定的bitNum来划分位数，用split分割
	* @attention 20191009：反转不是简单的将bit位直接反转，而是先按照bitNum将bit信息分割成单独块后再以块为单位反转
	* @param[in] src 待转换的十进制数值(可为unsigned short、unsigned int类型)
	* @param[in] bitNum 按照指定数量的bit划分
	* @param[in] splitCode 分割符
	* @param[in] reversed 是否需要反转显示，默认为false(默认按照高位在左侧，低位在右)
	*/
	template<class T>
	QString ScaleSwitcher::switchOctToBinary(T src, int bitNum, QString splitCode, bool reversed)
	{
		int loopTime = sizeof(T) * 8;
		QString result;

		if (reversed) {
			QString t_unit;
			for (int i = loopTime - 1; i >= 0; i--) {
				t_unit.append(QString::number((src >> i) & 1));

				if (i % bitNum == 0) {
					if (reversed) {
						result.prepend(t_unit);

						if (i != 0)
							result.prepend(splitCode);
					}
					t_unit.clear();
				}
			}
		}
		else {
			for (int i = loopTime - 1; i >= 0; i--) {
				if (i < loopTime - 1 && i % bitNum == 1) {
					result.append(splitCode);
				}
				result.append(QString::number((src >> i) & 1));
			}
		}

		return result;
	}

}//namespace Base

#endif // SCALESWITCHER_H
