/*!
 *  @brief     主题清单
 *  @details   列举出框架发布的主题号，主要由插件发送消息至框架。
 *  @author    wey
 *  @version   1.0
 *  @date      2019.06.20
 *  @warning   此接口描述了框架允许接收外部信息的清单
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TOPICLIST_H_Main_2019_09_29
#define TOPICLIST_H_Main_2019_09_29

namespace Core{

namespace SystemMsg{

//各个插件->框架：更新网络解析数量
const char plugin_topic_parsedlimit[] = "plugin_parsedlimit";
const char plugin_topic_networkParamterUpdate[] = "plugin_networkparameterupdate";

//系统->插件
const char system_topic_networkchanged[] = "system_net_changed";

} // namespace Topic

} //namespace Core

#endif // TOPICLIST_H
