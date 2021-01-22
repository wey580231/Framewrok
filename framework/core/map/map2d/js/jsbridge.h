#ifndef JSBRIDGE_H
#define JSBRIDGE_H

#include <QObject>

namespace Core{

namespace MapWeb2D {

class JSBridge : public QObject
{
    Q_OBJECT
public:
    explicit JSBridge(QObject *parent = 0);
    ~JSBridge();

public slots:
    void jsCallback(const QString msg);
    void jsCallbackForPainter(const QString msg);
    Q_INVOKABLE void jsCallbackForGetLatLng(const QString msg);
    void jsCallbackForSaveColorInfo(const QString msg);
    void jsCallbackForDeleteColorInfo(const QString msg);
    void jsCallbackMarkerDragEndPosition(const QString msg);
    void jsCallbackMarkerContextmenu(const QString msg);

    void jsCallbackScreenPointToGeo(const QString msg);

signals:
    /**< 信号：js向Qt界面传递消息 */
    void jsMsg(const QString msg);
    void jsMsgForPainter(const QString msg);
    void jsMsgForGetLatLng(const QString msg);
    void jsMsgForSaveColorInfo(const QString msg);
    void jsMsgForDeleteColorInfo(const QString msg);
    void jsMsgForMarkerDragEndPosition(const QString msg);
    void jsMsgForMarkerContextmenu(const QString msg);

    void jsMsgForGetSite(const QString msg);
};

} //namespace MapWeb2D

} //namespace Core

#endif // JSBRIDGE_H
