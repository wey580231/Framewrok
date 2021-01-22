#include "jsbridge.h"

#include <QDebug>

namespace Core{

namespace MapWeb2D {

JSBridge::JSBridge(QObject *parent) : QObject(parent)
{

}

JSBridge::~JSBridge()
{

}


/**< js传递目标模型信息 */
void JSBridge::jsCallback(const QString msg)
{
    emit jsMsg(msg);
}

/*! < js传递海图上点击的图层实例ID给绘图插件，用于删除点击的图层颜色信息QMap节点 > */
void JSBridge::jsCallbackForDeleteColorInfo(const QString msg)
{
    emit jsMsgForDeleteColorInfo(msg);
}

/*! < js传递海图上点击的图层实例ID给绘图插件，用于存储点击的图层颜色信息QMap节点 > */
void JSBridge::jsCallbackForSaveColorInfo(const QString msg)
{
    emit jsMsgForSaveColorInfo(msg);
}

/*! < js传递图形动态数据信息给绘图插件 > */
void JSBridge::jsCallbackForPainter(QString msg)
{
    emit jsMsgForPainter(msg);
}

/*! < js传递地图右键点击的实时位置 > */
void JSBridge::jsCallbackForGetLatLng(const QString msg)
{
    emit jsMsgForGetLatLng(msg);
}

/**
 * @brief 海图中图元目标拖动后产生的返回位置
 */
void JSBridge::jsCallbackMarkerDragEndPosition(const QString msg)
{
    emit jsMsgForMarkerDragEndPosition(msg);
}

/**
 * @brief 兵力对象右键事件
 * @param msg ：接口内容字符串
 * clientX: 123; clientY:456; lng:123.456;lat:23.123;id:1001;
 */
void JSBridge::jsCallbackMarkerContextmenu(const QString msg)
{
    emit jsMsgForMarkerContextmenu(msg);
}

/**
 * @brief 根据屏幕坐标获取海图位置
 * @param msg
 */
void JSBridge::jsCallbackScreenPointToGeo(const QString msg)
{
    emit jsMsgForGetSite(msg);
}

} //namespace MapWeb2D

} //namespace Core
