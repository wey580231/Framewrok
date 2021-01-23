#include "rhttp.h"

#include <QDebug>
#include <QBuffer>

namespace Network {

RHttp::RHttp():QObject(),m_successHandler(nullptr),m_errorHandler(nullptr),m_accessManager(nullptr)
{

}

RHttp::~RHttp()
{
    if(m_accessManager)
        delete m_accessManager;
}

void RHttp::reset()
{
    m_request = QNetworkRequest();
    m_successHandler = nullptr;
    m_errorHandler = nullptr;
}

/*!
 * @brief 设置访问基本路径，如【http://192.168.31.134:8080】
 * @param[in] url 基本路径
 */
void RHttp::setBaseUrl(QString url)
{
    m_baseUrl = url;
}

/*!
 * @brief 设置restfule风格接口【/oauth/token】，可以与baseurl拼接成一个完整的地址链接
 * @param[in] url restful访问地址
 */
void RHttp::setRestUrl(QString url)
{
    m_url = url;
}

RHttp &RHttp::setHeader(QNetworkRequest::KnownHeaders name, QString value)
{
    m_request.setHeader(name,value);
    return *this;
}

RHttp &RHttp::setHeader(QPair<QString, QString> keyValue)
{
    m_request.setRawHeader(keyValue.first.toLocal8Bit(),keyValue.second.toLocal8Bit());
    return *this;
}

RHttp &RHttp::setHeader(QString name, QString value)
{
    m_request.setRawHeader(name.toLocal8Bit(),value.toLocal8Bit());
    return *this;
}

void RHttp::get(NetworkReplyHandler successHandler, NetworkReplyErrorHandler errorHandler)
{
    processRequest(R_GET,"",successHandler,errorHandler);
}

void RHttp::post(QByteArray bodyData, NetworkReplyHandler successHandler, NetworkReplyErrorHandler errorHandler)
{
    processRequest(R_POST,bodyData,successHandler,errorHandler);
}

void RHttp::deleteResource(NetworkReplyHandler successHandler, NetworkReplyErrorHandler errorHandler)
{
    processRequest(R_DELETE,"",successHandler,errorHandler);
}

void RHttp::put(QByteArray bodyData, NetworkReplyHandler successHandler, NetworkReplyErrorHandler errorHandler)
{
    processRequest(R_PUT,bodyData,successHandler,errorHandler);
}

void RHttp::patch(QByteArray bodyData, NetworkReplyHandler successHandler, NetworkReplyErrorHandler errorHandler)
{
    processRequest(R_PATCH,bodyData,successHandler,errorHandler);
}

void RHttp::respError(QNetworkReply::NetworkError error)
{
    if(m_errorHandler)
        m_errorHandler(error);
    else
        qDebug()<<error;
}

QString RHttp::packRequestURL()
{
    if(!m_baseUrl.isEmpty())
        return m_baseUrl + m_url;
    return m_url;
}

void RHttp::processRequest(RHttp::RequestType type, QByteArray data, NetworkReplyHandler successHandler, NetworkReplyErrorHandler errorHandler)
{
    QString t_requestUrl = packRequestURL();

    m_successHandler = successHandler;
    m_errorHandler = errorHandler;

    if(m_accessManager == nullptr){
        m_accessManager = new QNetworkAccessManager();
    }

    m_request.setUrl(QUrl(t_requestUrl));

//qDebug()<<"url:"<<t_requestUrl;

    switch(type){
        case R_GET:{
                m_reply = m_accessManager->get(m_request);
            }
            break;
        case R_POST:{
                m_reply = m_accessManager->post(m_request,data);
            }
            break;
        case R_DELETE:{
                m_reply = m_accessManager->deleteResource(m_request);
            }
            break;
        case R_PUT:{
                m_reply = m_accessManager->put(m_request,data);
            }
            break;
        case R_PATCH:{
                QBuffer tbuffer(&data);
                m_reply = m_accessManager->sendCustomRequest(m_request,QByteArray("PATCH"),&tbuffer);
            }
            break;
        default:
            break;
    };

    connect(m_reply,&QNetworkReply::readyRead,[&](){
        if(m_successHandler)
            m_successHandler(m_reply);
    });

    connect(m_reply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(respError(QNetworkReply::NetworkError)));
}


} //namespace Network
