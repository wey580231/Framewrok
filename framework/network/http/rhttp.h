/*!
 *  @brief    封装Http请求
 *  @details  对http的访问进一步封装，简化使用
 *  @author    wey
 *  @version   1.0
 *  @date      2019.11.12
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RHTTP_H_2019_12_10
#define RHTTP_H_2019_12_10

#include <functional>
#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>

class QNetworkAccessManager;

namespace Network {

typedef std::function<void(QNetworkReply *)> NetworkReplyHandler;
typedef std::function<void(QNetworkReply::NetworkError)> NetworkReplyErrorHandler;

class RHttp : public QObject
{
    Q_OBJECT
public:
    explicit RHttp();
    ~RHttp();

    void reset();

    void setBaseUrl(QString url);
    void setRestUrl(QString url);

    RHttp& setHeader(QNetworkRequest::KnownHeaders name,QString value);
    RHttp& setHeader(QPair<QString,QString> keyValue);
    RHttp& setHeader(QString name,QString value);

    void get(NetworkReplyHandler successHandler = nullptr,NetworkReplyErrorHandler errorHandler = nullptr);
    void post(QByteArray bodyData,NetworkReplyHandler successHandler = nullptr,NetworkReplyErrorHandler errorHandler = nullptr);
    void deleteResource(NetworkReplyHandler successHandler = nullptr,NetworkReplyErrorHandler errorHandler = nullptr);
    void put(QByteArray bodyData,NetworkReplyHandler successHandler = nullptr,NetworkReplyErrorHandler errorHandler = nullptr);
    void patch(QByteArray bodyData,NetworkReplyHandler successHandler = nullptr,NetworkReplyErrorHandler errorHandler = nullptr);

private slots:
    void respError(QNetworkReply::NetworkError error);

private:
    enum RequestType{
        R_GET,
        R_POST,
        R_DELETE,
        R_PUT,
        R_PATCH
    };

private:
    QString packRequestURL();
    void processRequest(RequestType type,QByteArray data = "",NetworkReplyHandler successHandler = nullptr,NetworkReplyErrorHandler errorHandler = nullptr);

private:
    QString m_url;
    QString m_baseUrl;

    QNetworkRequest m_request;
    QNetworkReply * m_reply;
    QNetworkAccessManager * m_accessManager;

    NetworkReplyHandler m_successHandler;
    NetworkReplyErrorHandler m_errorHandler;
};

} //namespace Network

#endif // RHTTP_H
