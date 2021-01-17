/*!
 *  @brief     插件信息邮箱
 *  @details   1.负责接收全局邮箱投递的数据;
 *             2.负责向全局邮箱投递数据;
 *  @author    wey
 *  @version   1.0
 *  @date      2019.01.18
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef PLUGINMAILBOX_H
#define PLUGINMAILBOX_H

#include <core/core_global.h>

#include "abstractmailbox.h"

namespace Core{

class RComponent;

class CORESHARED_EXPORT PluginMailBox : public AbstractMailBox
{
    Q_OBJECT
public:
    explicit PluginMailBox(RComponent * comp,QObject * parent = 0);
    ~PluginMailBox();

    /*!< 设置单词获取邮件数量 */
    void setPollSize(int maxSize = 100);
    int getMailSize();

    /*!< 手动获取邮件 */
    void poll();

    /*!< 投递邮件 */
    void recvMail(Datastruct::PluginMessage *mail,Base::ElementPriority pty = Base::NormalPriority) override;

signals:
    void dataReady(Datastruct::PluginMessageList maillist);

protected:
    void run() override;

private:
    QString pluginId;       /*!< 所属插件Id */
    RComponent * component;
    int pollSize;           /*!< 批量获取邮件大小，默认为1 */
};

} //namespace Core

#endif // PLUGINMAILBOX_H
