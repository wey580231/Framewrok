/*!
 *  @brief     组件间消息通信类型
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.26
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note 20190326:wey:调整系统消息类型为const char *
 */
#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

namespace MessageType
{
/*!
 *  @brief 消息类型
 *  @details
 */
const char MESS_LAN_CHANGED[]  = "mess_lan_changed";                /*!< 语言切换 */
const char MESS_NETWORK_CHANGED [] = "mess_network_changed";        /*!< 网络状态改变 */

}

#endif // MESSAGETYPE_H
