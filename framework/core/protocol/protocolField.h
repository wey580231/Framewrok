/*!
 *  @brief     应用层协议文件
 *  @file      protocoldata.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.15
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 *          20190126:wey:增加对设置了repeat属性字段值获取
 */
#ifndef PROTOCOLFIELD_H
#define PROTOCOLFIELD_H

#include <QVariant>
#include <QVector>
#include <QWidget>

#include <core/core_global.h>

namespace Core{

#pragma pack(push)
#pragma pack(1)
namespace Datastruct  {
/****************************************xml插件协议字段设置************************************************************/

/*!
 *  @brief 控件类型
 */
enum CORESHARED_EXPORT ControlType{
    Empty = 0,

    /*! Item的type字段控件类型 */
    ComboBox = 1,        /*!< 下拉框 */
    CheckBox,            /*!< 复选框 */
    RadioButton,         /*!< 单选框 */
    TextEdit,            /*!< 文本输入框 */
    ValueIntEdit,        /*!< 整形输入框 */
    ValueFloatEdit,      /*!< 浮点形输入框 */
    ValueLongLongEdit,   /*!< 长整形输入框-8字节:20200117 */
    DateEdit,            /*!< 日期输入框 */
    TimeEdit,            /*!< 时间输入框 */
    DateTimeEdit,        /*!< 日期时间输入框:wey:20200309 */
    IpWidget,            /*!< ip地址输入类型*/
    MacWidget,           /*!< mac地址输入类型*/

    /*! 容器框 */
    Tab,                 /*!< tab */
    Dialog,              /*!< 对话框 */
    Widget,              /*!< 普通框 */
    Groupbox,            /*!< 组合框*/
    Table,               /*!< 表格 */
    List,                /*!< 列表 */

    /*! 协议类型*/
    DiffValid,           /*!< 差值有效,有效数值为这一次的数据减去上一次的数据 */

    Count,               /*!< 计数类型 */
    Statistic,           /*!< 统计类型 */
    FrameType,           /*!< 帧类型 */
    Mark,                /*!< 标识符类型 */
    RegexpBit            /*!< 20190507:wey:格式化输出位，铁塔重加载应答结果 */
};

/*!
 *  @brief 字段值状态类型
 *  @date 20190617
 *  @details HLS的标签包中，需要对设备状态值进行结果运算，初步分为心跳报值和状态值两种。每个设备包含多个此种状态值，只有全部正常，设备才正常，否则设备故障。
 */
enum StateType{
    StateNone,
    HeartValue,         /*!< 心跳类型值(每个包中数据在变换，若10个周期内数据未更新，则认为设备故障)*/
    StateValue,         /*!< 状态类型值(default字段需设置，将接收到的值与default设置的值比较，相同设备正常，否则设备故障) */
    RangeValue,         /*!< 范围类型值(range字段需要设置，将收到的值与range比较)*/
    LonLatValue      /*!< 时分秒显示(用于经纬度格式化输出:20191014:wey:舰载课题)*/
};

struct  PubHead{
    PubHead():widget(NULL){}
    PubHead(const PubHead& newData);
    virtual ~PubHead(){
        if(widget)
            delete widget;
    }

    QWidget  * widget;     /*!< 实际生成的控件 */
    ControlType  type;     /*!< 字段类型 */
};

/*!
 * @brief 数据范围
 * @details 根据Field的type来转换为对应的值，可转换的包括int、double、date等
 * 20190103 转移到此位置，为了适应bit对range属性解析<hca>
 */
struct DataRange{
    bool isEmpty()const{return minValue.isEmpty() && maxValue.isEmpty();}

    bool operator == (const DataRange& range)const{
        return (this->minValue == range.minValue)&&(this->maxValue == range.maxValue);
    }
    QString minValue;
    QString maxValue;
    QString steppingValue;		/*!< 步进(QSpinBox、QDoubleSpinBox有效)*/
};

/*!
 * @brief 单个字节或位的通用描述信息
 * @details
 */
struct MetaData{
    MetaData():type(Empty),index(0),enable(true),visible(false),merged(false),weight(1),precision(0),isSigned(false),base(10)
      ,isComboxItemUserData(false),defaultListIndex(0),m_stateType(StateNone){
        name.clear();
        defaultValue.clear();
    }

    MetaData(const MetaData& newData){
        this->name = newData.name;
        this->type = newData.type;
        this->index = newData.index;
        this->enable = newData.enable;
        this->visible = newData.visible;
        this->merged = newData.merged;
        this->weight = newData.weight;
        this->precision = newData.precision;
        this->unit = newData.unit;
        this->displayText = newData.displayText;
        this->defaultValue = newData.defaultValue;
        this->defaultValShow = newData.defaultValShow;
        this->value = newData.value;
        this->list = newData.list;
        this->dataList = newData.dataList;
        this->isComboxItemUserData = newData.isComboxItemUserData;
        this->defaultListIndex = newData.defaultListIndex;
        this->isSigned = newData.isSigned;
        this->base = newData.base;
        this->range = newData.range;
        this->m_stateType = newData.m_stateType;
        this->m_dateTimeDisplayFormat = newData.m_dateTimeDisplayFormat;
        this->m_memOrder = newData.m_memOrder;
    }

    MetaData & operator=(const MetaData & newData){
        this->name = newData.name;
        this->type = newData.type;
        this->index = newData.index;
        this->enable = newData.enable;
        this->visible = newData.visible;
        this->merged = newData.merged;
        this->weight = newData.weight;
        this->precision = newData.precision;
        this->unit = newData.unit;
        this->displayText = newData.displayText;
        this->defaultValue = newData.defaultValue;
        this->defaultValShow = newData.defaultValShow;
        this->value = newData.value;
        this->list = newData.list;
        this->dataList = newData.dataList;
        this->isComboxItemUserData = newData.isComboxItemUserData;
        this->defaultListIndex = newData.defaultListIndex;
        this->isSigned = newData.isSigned;
        this->base = newData.base;
        this->range = newData.range;
        this->m_stateType = newData.m_stateType;
        this->m_dateTimeDisplayFormat = newData.m_dateTimeDisplayFormat;
        this->m_memOrder = newData.m_memOrder;
        return *this;
    }
    bool operator==(const MetaData & newData)const{
        return (this->name == newData.name)&&
        (this->type == newData.type)&&
        (this->index == newData.index)&&
        (this->enable == newData.enable)&&
        (this->visible == newData.visible)&&
        (this->merged == newData.merged)&&
        (this->weight == newData.weight)&&
        (this->precision == newData.precision)&&
        (this->unit == newData.unit)&&
        (this->displayText == newData.displayText)&&
        (this->defaultValue == newData.defaultValue)&&
        (this->defaultValShow == newData.defaultValShow)&&
        (this->value == newData.value)&&
        (this->list == newData.list)&&

        (this->dataList == newData.dataList)&&
        (this->isComboxItemUserData == newData.isComboxItemUserData)&&
		(this->base == newData.base)&&
        (this->defaultListIndex == newData.defaultListIndex)&&
        (this->isSigned == newData.isSigned)&&
        (this->range == newData.range);
    }
    virtual QVariant getShowVal(QVariant orignalVal) = 0;	/*!< 20190709 给定原始值，获取显示值 */

    QString  name;        /*!< 显示信息 --可以为空*/
    ControlType  type;    /*!< 字段类型 */
    //待定，删除下个数据
    int index;            /*!< 字段唯一标识符，初始为0。无需从xml中解析，新添加字段后，此值+1 */

    bool enable;          /*!< 控件是否可用 */
    bool visible;         /*!< 是否可见，可控制此字段是否显示在表格等控件上 */
    bool merged;          /*!< 是否为待合并的字段，可控制此字段是否显示在表格等控件上 */
    double weight;        /*!< 权值 */
    int  precision;       /*!< 精度:默认为0,当>=1时，保存指定有效位 20190613:wey */
    bool   isSigned;      /*!< 有无符号位,默认false-无符号 */
    int base;             /*!< 进制 -默认10进制 20190528:wey*/

    QString unit;           /*!< 单位 */
    QString displayText;    /*!< 显示文本 */
    QString defaultValue;   /*!< 默认实际值 */
    QString defaultValShow; /*!< 默认显示值 */
    QVariant value;         /*!< 控件中显示值 */

    QStringList list;      /*!< type为ComboBox时，保存下拉框中子项名称,【20190508:此项不保存】 */
    QStringList dataList;  /*!< isComboxItemUserData为true时，保存每一项的默认值 */
    bool isComboxItemUserData;    /*!< 是否用户指定了列表中的值每一项的值，默认为false。
                                    true： 将用户指定的值设置为item的userdata
                                    false: 将自增的索引设置为item的userdata
                                  */
    int defaultListIndex;  /*!< combox默认索引，支持配置设定 */
    DataRange range;      /*!< 数值范围 */

    StateType m_stateType;  /*!< 20190617:字段状态类型。 【不参与保存】 */
    QString m_groupId;      /*!< 20191022:字段所属分组，相同分组的字段会被划分至同一集合中【不参与保存】 */

    QString m_dateTimeDisplayFormat;        /*!< 20200309:wey:日期时间显示格式 */
    QString m_memOrder;                     /*!< 20200309:wey:日期时间内存序：年y、月M、日d、时h、分m、秒s，
                                                 若h8-m8-s16[1000]-d8-M8-y16
                                                 s16[1000]:表示秒s占用16bit，并且排列在分之后，其权值为1000
                                                 h8：表示时h占用8bit，排在第一位，权值为1
                                                */
};

/*!
 *  @brief 位基本数据信息描述
 *  @details
 */
struct CORESHARED_EXPORT BitData:public MetaData{
    BitData():MetaData(){}
    BitData(const BitData & newData):MetaData(newData){
        this->startPos = newData.startPos;
        this->last = newData.last;
    }

    BitData & operator= (const BitData & newData){
        MetaData::operator =(newData);
        this->startPos = newData.startPos;
        this->last = newData.last;
        return *this;
    }
    bool operator== (const BitData &newData)const{
        return (MetaData::operator ==(newData)&&(this->startPos == newData.startPos)
                &&(this->last == newData.last));
    }
    bool operator != (const BitData &newData)const{
        return  !(this->operator ==(newData));
    }

    uint read(const QVariant &byte,int len)const ;
    QVariant getValue(const QVariant &byte, int len=1, bool reality = false)const;
    QVariant getShowVal(QVariant orignalVal)override;

    friend QDataStream & operator<< (QDataStream & stream,const BitData & field);
    friend QDataStream & operator>> (QDataStream & stream,BitData & field);

    int startPos;         /*!< 起始位 */
    int last;             /*!< 持续位数 */
};

/*!
 * @brief 单个位字段
 * @details
 */
struct CORESHARED_EXPORT Bit : public PubHead
{
    Bit():PubHead(){}
    Bit(const Bit & newData):PubHead(newData){
        this->data = newData.data;
    }

    friend QDataStream & operator<< (QDataStream & stream,const Bit & field);
    friend QDataStream & operator>> (QDataStream & stream,Bit & field);

    QVariant getValue(const QVariant &byte, int len=1, bool reality = false)const ;
    BitData  data;

private:
    uint read(const QVariant &byte,int len = 1)const;
};


typedef QList<Bit *> BitList;

/*!
 *  @brief 位字段集合
 */
struct CORESHARED_EXPORT BitField : public PubHead{

    BitField():PubHead(){}

    ~BitField(){
        std::for_each(bitList.begin(),bitList.end(),[&](const Bit * bit){
            if(bit)
                delete bit;
        });
    }

    friend QDataStream & operator<< (QDataStream & stream,const BitField & field);
    friend QDataStream & operator>> (QDataStream & stream,BitField & field);

    BitList bitList;        /*!< 位字段集合 */
};

typedef QVector<QVariant> VariantVector;

/*!
 *  @brief 单个字段内容数据
 *  @warning (字节数-符号位) 与 (位操作集合) 这两对值不能同时使用，属于互斥关系
 */
struct CORESHARED_EXPORT Data_Word : public MetaData{
public:
    Data_Word():MetaData(),bitOperator(false),bytes(0),repeat(1),bytes_Accumulate(0){
    }

    Data_Word(const Data_Word & newWord):MetaData(newWord)
    {
        this->bitOperator = newWord.bitOperator;
        this->bytes = newWord.bytes;
        this->isSigned = newWord.isSigned;
        this->repeat = newWord.repeat;
        this->bytes_Accumulate = newWord.bytes_Accumulate;
        this->m_bitStretch = newWord.m_bitStretch;
        this->m_displayFormat = newWord.m_displayFormat;

        std::for_each(newWord.bits.bitList.begin(),newWord.bits.bitList.end(),[&](Bit * newWord){
            Bit * bit = new Bit(*newWord);
            bits.bitList.append(bit);
        });
    }

    Data_Word & operator= (const Data_Word & newWord)
    {
        if(this == &newWord)
            return *this;

        MetaData::operator =(newWord);

        this->bitOperator = newWord.bitOperator;
        this->bytes = newWord.bytes;
        this->isSigned = newWord.isSigned;
        this->repeat = newWord.repeat;
        this->bytes_Accumulate = newWord.bytes_Accumulate;
        this->m_bitStretch = newWord.m_bitStretch;
        this->m_displayFormat = newWord.m_displayFormat;

        bits.bitList.clear();
        std::for_each(newWord.bits.bitList.begin(),newWord.bits.bitList.end(),[&](Bit * newWord){
            Bit * bit = new Bit(*newWord);
            bits.bitList.append(bit);
        });

        return *this;
    }

    __declspec(dllexport) friend QDataStream & operator<<(QDataStream & stream,const Data_Word & data);
    __declspec(dllexport) friend QDataStream & operator>>(QDataStream & stream,Data_Word & data);

    QVariant getShowVal(QVariant orignalVal);

    QVariant getValue(const QByteArray &array,bool reality = false,bool isMatchedData = false)const ;
    QVariant getValue(const QByteArray &array,int bitIndex,bool reality = false, bool useThisArray = false)const ;
    VariantVector getRepeatValue(const QByteArray &array, bool reality) const;

    bool bitOperator;     /*!< 位操作字段，当bits中不为0，此值为true */
    ushort bytes;         /*!< 字节数 */
    BitField bits;        /*!< 位操作集合 */
    int repeat;           /*!< 重复的次数，此字段之后连续出现的次数 -1 标识根据前后的标识符类型进行判定*/


    //20190507:wey:铁塔项目需要将字节内分整数、小数格式化输出，在类型为
    QString m_bitStretch;           /*!< 从字节低位开始，每个显示位占用的位长度比，若长度为1字节的数据，高四位表示整数，低四位表示小数，可写成4:4 */
    QString m_displayFormat;        /*!< 占位符输出格式，序号从1开始，形如【%2:%1】。输出时将 @see m_bitStretch 字段内容填充至对应占位符 */

    ushort bytes_Accumulate;    /*!< 累计字节数 - 在单个协议中定位该数据*/
};

/*!
 *  @brief  协议字段信息
 *  @details 协议每项的信息描述
 */
struct Field : public PubHead{
    Field():PubHead(){}
    ~Field(){}

    Data_Word data;        /*!< 字段属性 */
};

struct ByteValue{
    QVariant value;
};
typedef QList<ByteValue> ByteValues;


struct Field_Protocol{
    Field_Protocol():index(0),protocolIndex(0),valList(NULL){}
    int protocolIndex;          /*!< 所属协议在所有协议中索引 */
    int index;                  /*!< 同一协议中字段索引 */
    ByteValue signalValue;      /*!< 适用于单个字段值 */
    ByteValues * valList;       /*!< 适用于同一个字段被重复多次;字段属于bit位操作 */
};

typedef QList<Field_Protocol> FieldValues;

/*!
 *  @brief  保存解析后的结果值
 */
struct ParsedResult{
    FieldValues fieldResults;
    QList<ParsedResult * > results;
};


/*!
 * @brief 基于协议的原始TCP数据包单元
 * @details 网络层接收接收一个完整的数据包后，将数据打包并标记类型，交由解析线程处理
 * @attention QByteArray类已经提供了移动(QByteArray::operator=(QByteArray &&other))，保存数据指针的方式可以去掉，直接使用对象。
 */
struct CORESHARED_EXPORT ProtocolArray{
    explicit ProtocolArray():protocolType(-1),socketId(-1){
        data = new QByteArray();
    }

    ProtocolArray(const ProtocolArray& array){
        this->networkId = array.networkId;
        this->protocolType = array.protocolType;
        this->socketId = array.socketId;
        this->ip = array.ip;

        this->data = array.data;
       (const_cast<ProtocolArray&> (array)).data = NULL;
        this->protocolType = array.protocolType;
    }

    ~ProtocolArray(){
        if(data)
            delete data;
    }

    ProtocolArray& operator =(const ProtocolArray& array){
        if(&array ==  this)
            return *this;

        this->networkId = array.networkId;
        this->protocolType = array.protocolType;
        this->socketId = array.socketId;
        this->ip = array.ip;

        this->data = array.data;
        (const_cast<ProtocolArray&> (array)).data = NULL;
        this->protocolType = array.protocolType;
        return *this;
    }

    QString networkId;          /*!< 数据接收服务器ID-对应网络设置中网络标识 */
    QString ip;                 /*!< 数据包所属客户端IP */
    int protocolType;           /*!< 数据协议类型 */
    int socketId;               /*!< 客户端socket连接标志 */
    QByteArray * data;          /*!< 原始数据指针，为了减少拷贝，采用指针形式 */
};

/*!
 *  @brief 基于原始数据的UDP/TCP数据包单元
 *  @details 该单元面向于无通用协议情形，udp/tcp接收原始数据后，由插件自身解析
 */
struct CORESHARED_EXPORT OriginalArray{
    QString mid;                /*!< 组件ID */
    QString ip;                 /*!< 数据包所属客户端IP */
    QByteArray data;            /*!< 原始数据块 */
};

struct CORESHARED_EXPORT NodeInfo{
    const QString window = "window";
    const QString name = "name";
    const QString width = "width";
    const QString height = "height";
    const QString layout = "layout";
    const QString taskId = "taskId";
    const QString type = "type";
    const QString autoLayout = "autolayout";
    const QString sendNet = "sendNet";

    const QString widget = "widget";
    const QString groupBox = "groupbox";

    const QString item = "item";
    const QString items = "items";

    const QString nodeShow = "show";
    const QString nodeColumn = "column";
    const QString nodeEnabled = "enabled";

    const QString itemName = "name";
    const QString itemBytes = "bytes";
    const QString itemSigned = "signed";
    const QString itemEnable = "enable";
    const QString itemVisible = "visible";
    const QString itemMerged = "merged";
    const QString itemBits = "bits";
    const QString itemWeight = "weight";
    const QString itemText = "text";
    const QString itemPrecision = "precision";
    const QString itemUnit = "unit";
    const QString itemRange = "range";
    const QString itemType = "type";
    const QString itemComboxList = "list";
    const QString itemComboxIndex = "index";        /*!< 字段list的属性 */
    const QString itemComboxDefaultItem = "default";

    const QString itemStateType = "statetype";      /*!< 20190617:wey:hls标签包 */

    const QString itemBitStretch = "bitstretch";        /*!< 20190507:wey */
    const QString itemDisplayFormat = "displayformat";

    const QString itemGroup = "group";          /*!< 20191022:分组ID */

    /*
     *default:默认实际值，界面上显示的值，即为实际存储的值
     *default_SHow:默认显示值，仅显示，当存在该字段时，需要将界面上的值与该值进行计算后，得到实际的值
     */
    const QString itemDefaultValue = "default";           /*!< 字段的默认实际值 */
    const QString itemDefaultValue_Show = "defaultshow";  /*!< 字段的默认显示值 */

    const QString itemDateTimeFormat = "datetimeformat";        /*!< 20200309:日期默认显示个数 */
    const QString itemDateTimeOrder = "datetimeorder";          /*!< 20200309:日期内存显示顺序 */
	
    const QString itemMax = "max";
    const QString itemMin = "min";
	const QString itemStepping = "stepping";        /*!< 20191225:spinbox的步进*/
	
    const QString itemRepeat = "repeat";
    const QString itemBase = "base";                /*!< 进制 */

    const QString bitStart = "start";
    const QString bitLast = "last";
};


/*!
 *  @brief xml文件中类型名称
 */
struct CORESHARED_EXPORT WidgetType{

    /*!< Item显示控件类型 */
    QString combox = "combox";
    QString checkBox = "checkBox";
    QString radioButton = "radiobutton";
    QString textEdit = "textedit";
    QString valueint = "valueint";
    QString valuefloat = "valuefloat";
    QString valuelong = "valuelonglong";
    QString dateEdit = "dateedit";
    QString timeEdit = "timeedit";
    QString dateTimeEdit = "datetimeedit";  /*!< 20200309 设定日期和时间 */
    QString ipWidget = "ip";                /*!< IP地址控件，实例化RIpWidget 20190116新增 */
    QString macWidget = "mac";              /*!< MAC地址输入符号 20190116新增 */

    /*!< Widget类型 */
    QString dialog = "dialog";
    QString widget = "widget";
    QString table = "table";
    QString list = "list";


    /*** 协议控制类型 ***/
    const QString count = "count";
    const QString length = "length";
    const QString statistic = "statistic";      /*!< 统计信息 */
    const QString diffValid = "difference";     /*!< 差值有效 */
    const QString frameType = "frameType";      /*!< 帧类型--0起始帧，1中间帧，2终止帧，3单独帧 */
    const QString mark = "mark";                /*!< 标识符类型，用于判断字段是否为标记字段类型，如0x1ACF */

    const QString regexpBit = "regexpbit";
};


/*!
 *  @brief 字段状态类型
 */
struct StateTypeValue{
    const QString m_heartValue = "heartvalue";
    const QString m_rangeValue = "rangevalue";
    const QString m_stateValue = "statevalue";
    const QString m_lonlatValue = "lonlatvalue";
};

__declspec(dllexport) QVariant readBytes_FromBytesArray(const QByteArray &array,int &pos, int bytes, ControlType type = ValueIntEdit,bool isSigned = false);

} //namespace Datastruct

#pragma pack(pop)

} //namespace Core


#endif // PROTOCOLFIELD_H
