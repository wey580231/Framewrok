#include "protocolField.h"

#include <QDataStream>
#include <QDate>
#include <QTime>
#include <qmath.h>
#include <bitset>
#include <QDebug>
#include <base/util/rutil.h>

namespace Core{

namespace Datastruct {
//TODO 20181109保存时只需要保存字段的name、index以及索引就好，基本信息从协议中获取
QDataStream & operator<< (QDataStream &stream, const BitData & data)
{
    stream<<data.name;

    stream<<static_cast<int>(data.type);

    switch(data.type){
        case ComboBox :
            stream<<data.value.toInt();
            break;
        case CheckBox :
        case RadioButton :
            stream<<data.value.toBool();
            break;
        case TextEdit :
            stream<<data.value.toString();
            break;
        case ValueIntEdit :
            stream<<data.value.toInt();
            break;
        case ValueFloatEdit :
            stream<<data.value.toDouble();
            break;
        case ValueLongLongEdit :
            stream<<data.value.toLongLong();
            break;
        case DateEdit :
            stream<<data.value.toDate();
            break;
        case TimeEdit :
            stream<<data.value.toTime();
            break;
        case Empty :
        default:
            break;
    }

    stream<<data.index<<data.enable<<data.visible<<data.merged<<data.weight<<data.precision
         <<data.unit<<data.displayText<<data.defaultValue<<data.defaultListIndex
        <<data.startPos<<data.last;

    return stream;
}

QDataStream & operator>> (QDataStream &stream, BitData &data)
{
    stream>>data.name;

    int type = 0;
    stream >> type;
    data.type = static_cast<ControlType>(type);

    switch(data.type){
        case ComboBox :
        {
            int tmp = 0;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case CheckBox :
        case RadioButton :
        {
            bool tmp = false;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case TextEdit :
        {
            QString tmp;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case ValueIntEdit :
        {
            int tmp = 0;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case ValueFloatEdit :
        {
            double tmp = 0;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case ValueLongLongEdit :
        {
            qlonglong tmp = 0;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case DateEdit :
        {
            QDate tmp;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case TimeEdit :
        {
            QTime tmp;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case Empty :
        default:
            break;
    }

    stream>>data.index>>data.enable>>data.visible>>data.merged>>data.weight>>data.precision
            >>data.unit>>data.displayText>>data.defaultValue>>data.defaultListIndex
            >>data.startPos>>data.last;

    return stream;
}


QDataStream & operator<< (QDataStream & stream,const Bit & field)
{
    stream<<static_cast<int>(field.type);
    stream<<field.data;
    return stream;
}

QDataStream & operator>> (QDataStream & stream,Bit & field)
{
    int type = 0;
    stream >> type;
    field.type = static_cast<ControlType>(type);

    stream>>field.data;
    return stream;
}

QDataStream & operator<< (QDataStream &stream, const BitField &field)
{
    stream<<field.bitList.size();
    std::for_each(field.bitList.begin(),field.bitList.end(),[&](const Bit * data){
        stream<<(*data);
    });
    return stream;
}

QDataStream & operator>> (QDataStream &stream, BitField &field)
{
    int size = 0;
    stream >> size;
    for(int i = 0;i < size;i++){
        Bit * bit = new Bit;
        stream>>(*bit);
        field.bitList.append(bit);
    }
    return stream;
}

QDataStream & operator<<(QDataStream & stream,const Data_Word & data)
{
    stream<<data.name;

    stream<<static_cast<int>(data.type);
    switch(data.type){
        case ComboBox :
            stream<<data.value.toInt();
            break;
        case CheckBox :
        case RadioButton :
            stream<<data.value.toBool();
            break;
        case TextEdit :
            stream<<data.value.toString();
            break;
        case ValueIntEdit :
            stream<<data.value.toInt();
            break;
        case ValueFloatEdit :
            stream<<data.value.toDouble();
            break;
        case ValueLongLongEdit :
            stream<<data.value.toLongLong();
            break;
        case DateEdit :
            stream<<data.value.toDate();
            break;
        case TimeEdit :
            stream<<data.value.toTime();
            break;
        case IpWidget :
            stream<<data.value.toULongLong();
            break;
        case MacWidget :
            stream<<data.value.toString();
            break;
        case Empty :
        default:
            break;
    }

    stream<<data.index<<data.enable<<data.visible<<data.merged<<data.weight<<data.precision
         <<data.unit<<data.displayText<<data.defaultValue/*<<data.list*/<<data.defaultListIndex
        <<data.bitOperator<<data.bytes<<data.isSigned<<data.bits
       <<data.repeat<<data.range.minValue<<data.range.maxValue;

    return stream;
}

QDataStream & operator>>(QDataStream & stream,Data_Word & data)
{
    stream>>data.name;

    int type = 0;
    stream >> type;
    data.type = static_cast<ControlType>(type);

    switch(data.type){
        case ComboBox :
        {
            int tmp = 0;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case CheckBox :
        case RadioButton :
        {
            bool tmp = false;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case TextEdit :
        {
            QString tmp;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case ValueIntEdit :
        {
            int tmp = 0;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case ValueFloatEdit :
        {
            double tmp = 0;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case ValueLongLongEdit :
        {
            qlonglong tmp = 0;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case DateEdit :
        {
            QDate tmp;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case TimeEdit :
        {
            QTime tmp;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case IpWidget :
        {
            unsigned long long tmp;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case MacWidget :
        {
            QString tmp;
            stream>>tmp;
            data.value = QVariant::fromValue(tmp);
        }
            break;
        case Empty :
        default:
            break;
    }

    stream>>data.index>>data.enable>>data.visible>>data.merged>>data.weight>>data.precision
            >>data.unit>>data.displayText>>data.defaultValue/*>>data.list*/>>data.defaultListIndex
            >>data.bitOperator>>data.bytes>>data.isSigned>>data.bits
            >>data.repeat>>data.range.minValue>>data.range.maxValue;
    return stream;
}


/*!
 * @brief 从字节数组读取字节
 * @attention 20190411:wey:增加对超过8字节数据处理，直接返回指定数量的原始数据块
 * @param array 原始数据
 * @param type 数据字段类型
 * @param pos 数据截取起始偏移量
 * @param bytes 读取的字节数
 * @param isSigned 有无符号
 * @return 截取后的原始数据
 */
QVariant readBytes_FromBytesArray(const QByteArray &array, int &pos, int bytes, ControlType type, bool isSigned)
{
    if(pos < 0 || pos >= array.count() || (pos + bytes - 1) >= array.count())
        return QVariant();

    QVariant value;

    switch(bytes){
        case 1: {
            unsigned char temp = 0x00;
            memcpy(&temp,array.data()+ pos,1);
            pos += 1;
            value = QVariant(temp);
        }
            break;

        case 2:
            if(isSigned)
            {
                short temp = 0;
                memcpy(&temp,array.data()+ pos,2);
                pos += 2;
                value = QVariant(temp);
            }
            else
            {
                unsigned short temp = 0;
                memcpy(&temp,array.data()+ pos,2);
                pos += 2;
                value = QVariant(temp);
            }
            break;
        case 3:
        case 4:
        {
            if(type == ValueFloatEdit){
                float temp = 0;
                memcpy(&temp,array.data()+ pos,bytes);
                pos += bytes;
                value = QVariant(temp);
            }else{
                if(isSigned){
                    long temp = 0;
                    memcpy(&temp,array.data()+ pos,bytes);
                    pos += bytes;
                    value = QVariant(temp);
                }
                else{
                    unsigned int temp = 0;
                    memcpy(&temp,array.data()+ pos,bytes);
                    pos += bytes;
                    value = QVariant(temp);
                }
            }
        }
            break;
        case 5:
        case 6:
        case 7:
        case 8:{
            if(type == ValueFloatEdit){
                double temp = 0;
                memcpy(&temp,array.data()+ pos,bytes);
                pos += bytes;
                value = QVariant(temp);
            }else{
                if(isSigned){
                    long long  temp = 0;
                    memcpy(&temp,array.data()+ pos,bytes);
                    pos += bytes;
                    value = QVariant(temp);
                }
                else{
                    unsigned long long  temp = 0;
                    memcpy(&temp,array.data()+ pos,bytes);
                    pos += bytes;
                    value = QVariant::fromValue(temp);
                }
            }
        }
            break;

        default:
        {
            QByteArray t_result;
            t_result.append(array.data()+ pos,bytes);
            pos += bytes;
            value = QVariant::fromValue(t_result);
            break;
        }
    }
    return value;
}


QVariant Data_Word::getShowVal(QVariant orignalVal)
{
	//20190830tt敌我识别，外界误调用，导致14字节信息进入
	if(bytes > 8)//使用原始16进制显示
        return orignalVal.toByteArray().toHex();
		
	switch(type){
		case Core::Datastruct::IpWidget:
			return Base::RUtil::swithIntToIp(orignalVal.toInt());
		case Core::Datastruct::MacWidget:
            return Base::RUtil::swithLongToMac(orignalVal.toULongLong(),false);
		default:break;
    }

    QVariant value = orignalVal;
	
    if(bitOperator)
    {
        //NOTE 20191016 进一步优化 格式化输出 -- 持有位数据，却组合显示
       if(!m_displayFormat.isEmpty()){
               switch(bits.bitList.size()){
               case 2:return QString(m_displayFormat).arg(bits.bitList[0]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[1]->getValue(orignalVal,bytes,false).toString());
               case 3:return QString(m_displayFormat).arg(bits.bitList[0]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[1]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[2]->getValue(orignalVal,bytes,false).toString());
               case 4:return QString(m_displayFormat).arg(bits.bitList[0]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[1]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[2]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[3]->getValue(orignalVal,bytes,false).toString());
               case 5:return QString(m_displayFormat).arg(bits.bitList[0]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[1]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[2]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[3]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[4]->getValue(orignalVal,bytes,false).toString());
               case 6:return QString(m_displayFormat).arg(bits.bitList[0]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[1]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[2]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[3]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[4]->getValue(orignalVal,bytes,false).toString())
                                                     .arg(bits.bitList[5]->getValue(orignalVal,bytes,false).toString());
               default:return QVariant();
               }
        }
    }
    else
    {
        //格式化输出
        if(type == RegexpBit){
            ushort t_bits = bytes * 8;
            QStringList t_bitStretch = m_bitStretch.split(":");

            if(!m_displayFormat.isEmpty() && t_bitStretch.size() > 0){
                QString t_displayFormat = m_displayFormat;
                qulonglong t_value = value.toULongLong();

                int t_start = 0;

                while(!t_bitStretch.isEmpty()){
                    QString perbit = t_bitStretch.takeLast();
                    uint t_curBitLen = perbit.toUInt();
                    if(t_start + t_curBitLen <= t_bits){
                        int t_bitValue = 0;
                        for(int k = t_start + t_curBitLen - 1; k >= t_start ; k--){
                            uint t_tmp = t_value>>k & 0x01;
                            t_bitValue |= (t_tmp<<(k - t_start));
                        }

                        t_displayFormat = t_displayFormat.arg(t_bitValue);

                        t_start += t_curBitLen;
                    }
                }

                return  t_displayFormat;
            }
            return QVariant();
        }

        //下拉框类型
        if(!list.isEmpty()){
            if(dataList.isEmpty() && value.toInt() >= 0 && value.toInt() < list.count() )       //普通可选信息
                return list.at(value.toUInt());
            else
            {
                bool  is10BaseVal = false;		//判断默认值是否为10进制默认值
                if(dataList.size() > 0){
                    dataList.first().toInt(&is10BaseVal,10);
                }
                int base = is10BaseVal ? 10 : 16;
                int curValue = value.toInt();

                //下拉框中每项设置了default值，将字段值逐个和datalist中值比较，获取其真实的索引
                if(dataList.size() == list.size()){
                    for(int i = 0; i < dataList.count();i++){
                        int destVal = dataList.at(i).toInt(nullptr,base);
                        if(curValue == destVal )
                            return list.at(i);
                    }
                }
            }
        }
		
        if(type == Datastruct::ValueFloatEdit){
            if(bytes <= 4)
                value = (float)value.toFloat() * weight;
            else
                value = (double)value.toDouble() * weight;
        }
		
        //数据符号
        if(isSigned){
            if(weight != 1){
                value = (double)(value.toLongLong()*weight);
            }

            if(!defaultValShow.isEmpty()){
                value = value.toDouble() + defaultValShow.toFloat();
            }

            if(0 < precision)
                 return QString::number(value.toDouble(),'f',precision);
            else
                 return QString::number(value.toLongLong(),'f',0);
        }else{

            //数值数据 - 判断是否使用了进制限制
            switch(base){
                case 2:return  QString::number(value.toULongLong(),2);
                case 8:{
                    QString str = "0";
                    str += QString::number(value.toULongLong(),8);
                    return  str;
                }
                case 16:{
                    QString str = "0x";
                    str += QString::number(value.toULongLong(),16);
                    return  str;
                }
                default:break;
                }

                //权值
                if(weight != 1){
                    if(weight != (int)(weight))
                        value =(double)(value.toULongLong()*weight);
                    else
                        value = (value.toULongLong()*(int)weight);      //20190417yww 必须将权重分开写，NX出现过数据不对的问题
                }

                //默认显示值
                if(!defaultValShow.isEmpty()){

                    switch(value.type()){
                    case QVariant::Double:
                        value = value.toDouble() + defaultValShow.toFloat();
                        break;
                    default:
                        value = value.toULongLong() + defaultValShow.toInt();
                        break;
                    }
                }

                //显示精度(按照此处设置为准)
                if(0 < precision)
                     return QString::number(value.toDouble(),'f',precision);
                else
                     return QString::number(value.toULongLong(),'f',0);
            }
    }
    return QVariant();
}

/*!
 * @brief 从字节数组中读取值
 * @details 1.reality 为true时，获取原始数据值，不与权值做计算
 *          2.reality 为false时，将值与权值做乘法
 * @param array 原始块数据
 * @param reality true:是否获取原始数据（需调用者自己转换）
 *                false:获取经过权值转换的数据
 * @param isMatchedData false:从当前字段所在位置起获取数据
 *                      true:从位置0截取数据
 * @return
 */
QVariant Data_Word::getValue(const QByteArray &array, bool reality, bool isMatchedData) const
{    
    QVariant value;

    int curPos = 0;
    if(!isMatchedData){
        curPos = bytes_Accumulate;
    }

    value = readBytes_FromBytesArray(array,curPos,bytes,type,isSigned);

    if(reality)
        return value;
    else{
        value = const_cast<Data_Word*>(this)->getShowVal(value);
        //非位数据的格式化输出，却增加了Displayformat字段 -- 默认为前后缀显示[20200109：css:WRZ不需要]
//        if(!bitOperator && !m_displayFormat.isEmpty())
//            return QString(m_displayFormat).arg(value.toString());
    }

    return value;
}

/*!
 * @brief 截取原始数据
 * @param array 原始数据块
 * @param bitIndex 指定bit索引
 * @param reality 为true时获取原始值，false时将值与权值相乘
 * @param useThisArray 使用传入的array，即该array是该word中含有的信息
 * @return 该字段对应的原始数据
 */
QVariant Data_Word::getValue(const QByteArray &array, int bitIndex, bool reality, bool useThisArray) const
{
    QVariant value;

    int curPos = bytes_Accumulate;

    if(useThisArray)
        curPos = 0;

    value = readBytes_FromBytesArray(array,curPos,bytes,type,isSigned);

    if(bitOperator){
        if(bitIndex < bits.bitList.count() && bitIndex >=0 )
            return bits.bitList[bitIndex]->getValue(value,bytes,reality);
        else
            return QVariant();
    }

    return value;
}

/*!
* @brief 获取设置了repeat属性的字段内容
* @param[in] array 原始数据
* @param[in] reality true:获取字段原始数据值
*                    false:获取经过权值转换的数据
* @return 返回repeat次数的数据集合
*/
VariantVector Data_Word::getRepeatValue(const QByteArray &array, bool reality) const
{
    if(repeat > 0){
        VariantVector result(repeat);

        for(int i = 0;i < repeat;i++){
            QVariant value;

            int curPos = bytes_Accumulate + i * bytes;

            value = readBytes_FromBytesArray(array,curPos,bytes,type,isSigned);

            if(reality)
                result[i]= value;
            else {
                if(!list.isEmpty()){
                    if(value.toInt() < list.count()){
                        result[i]= (value.toUInt());
                    }
                }else{
                    result[i]= (value.toUInt()* weight);
                }
            }
        }
        return result;
    }

    return VariantVector();
}

/*!
 * @brief 从长度为len的byte数据中，截取一定位数据信息
 * @details 1.reality 为true时，获取原始数据值，不与权值做计算
 *          2.reality 为false时，将值与权值做乘法
 * @param[in] byte bit所在字段完整数据
 * @param[in] len 字段长度
 * @param[in] reality 是否返回解析后的数据
 * @return 当前截取的位数据
 */
QVariant BitData::getValue(const QVariant &byte, int len, bool reality) const
{
    uint  value = read(byte,len);   //TODO 此时的bit只支持无符号类型，此后应该还会更改
    if(reality){
        return value;
    }
    else{
        return const_cast<BitData*>(this)->getShowVal(value);
    }
}

/*!
 *  @brief 在字节中截取指定长度的bit数据
 */
template<class T>
ulong readBitDataFromBytes(T value,int bitStart,int bitLast){

    value >>= bitStart;

    ulong result = 0;
    int loop = 0;
    while(loop < bitLast){
        result += (value&0x01)*pow(2,loop);
        value>>=1;
        loop++;
    }
    return result;
}

QVariant BitData::getShowVal(QVariant orignalVal)
{
    QVariant value = orignalVal;
    
	//下拉框中每项设置了default值，将字段值逐个和datalist中值比较，获取其真实的索引
    uint t_uiComboxVal = value.toUInt();
    if(t_uiComboxVal >= 0 && !dataList.isEmpty() && list.size() == dataList.size())
    {
        bool  is10BaseVal = false;		//判断默认值是否为10进制默认值
        if(dataList.size() > 0){
            dataList.first().toInt(&is10BaseVal,10);
        }
        int base = is10BaseVal ? 10 : 16;

        for(int i = 0; i < dataList.count(); i++)
        {
            if(t_uiComboxVal == dataList.at(i).toInt(nullptr,base))
                return list.at(i);
        }
    }
    else{
        if(!list.isEmpty() && t_uiComboxVal <list.count() && t_uiComboxVal >=0){
            return list.at(t_uiComboxVal);
        }
    }

    if(weight != 1){
        value = value.toDouble()*weight;
    }

    if(!isSigned){
        //数值数据 - 判断是否使用了进制限制
        switch(base){
            case 2: return  QString::number(value.toULongLong(),2);
            case 8:{
                QString str = "0";
                str += QString::number(value.toULongLong(),8);
                return  str;
            }
            case 16:{
                QString str = "0x";
                str += QString::number(value.toULongLong(),16);
                return  str;
            }
            default:break;
        }
        if(0 < precision)
             return QString::number(value.toDouble(),'f',precision);
        else
             return QString::number(value.toLongLong(),'f',0);
    }
    else{
        if(0 < precision)
             return QString::number(value.toDouble(),'f',precision);
        else
             return QString::number(value.toLongLong(),'f',0);
    }

    return  QVariant();
}


/*!
 * @brief  从长度为len的byte数据中，根据bit的startpos、last截取数据
 * @attention 20190111:只能处理<=8 字节的数据信息;
 * @note    20190111:wey:修复因字段长度超过8字节，截取数据错误问题。
 * @details [1]向右移移(startPos)位
 *          [2]循环截取last长度，
 * @param[in] byte bit所在字段完整数据
 * @param[in] len bit所在字段数据长度
 * @return 当前截取的位数据
 */
uint BitData::read(const QVariant &byte, int len) const
{
    uint result = 0;
    if(len <= 4){
        uint originalData = byte.toUInt();
        result = readBitDataFromBytes<uint>(originalData,startPos,last);
    }else if(len <= 8){
        qulonglong originalData = byte.toULongLong();
        result = readBitDataFromBytes<qulonglong>(originalData,startPos,last);
    }
    return result;
}

PubHead::PubHead(const PubHead &newData)
{
    this->widget = newData.widget;
    this->type = newData.type;
}

QVariant Bit::getValue(const QVariant &byte, int len, bool reality) const
{
    return data.getValue(byte,len,reality);
}

uint Bit::read(const QVariant &byte, int len) const
{
    return data.read(byte,len);
}

} //namespace Datastruct

} //namespace Core
