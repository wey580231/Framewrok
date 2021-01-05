#include "map2dview.h"

#include <QWebChannel>
#include <QUrl>
#include <QDir>
#include <QDebug>

#include <core/file/programfilepath.h>
#include <core/global.h>
#include <core/file/globalconfigfile.h>

#include "js/jsbridge.h"

namespace Core{

namespace MapWeb2D{

MapView::MapView(QWidget* parent):QWebEngineView(parent)
{
    Core::ProgramFilePath filePath;
    configPath = filePath.configPath;
    configPath += "/";
}

MapView::~MapView()
{
    if(m_process){
//        m_process->kill();
    }
}

void MapView::initWebPage()
{
    QWebChannel* pWebChannel = new QWebChannel(this);
    JSBridge* pJSBridget = new JSBridge(this);
    pWebChannel->registerObject("bridge", pJSBridget);
    page()->setWebChannel(pWebChannel);

    QString urlPath = "file:///";
    urlPath += (configPath + Core::RGlobal::G_GlobalConfigFile->mapConfigInfo.mapPath);

    qDebug()<<urlPath;

    page()->setUrl(QUrl(urlPath));
}

void MapView::startWebService()
{
    m_process = new QProcess();
    connect(m_process,SIGNAL(stateChanged(QProcess::ProcessState)),this,SLOT(respProcessState(QProcess::ProcessState)));
    connect(m_process,SIGNAL(readyRead()),this,SLOT(sreadProcessOutput()));
    connect(m_process,SIGNAL(error(QProcess::ProcessError)),this,SLOT(respProcessError(QProcess::ProcessError)));

    QString exe = QString("cmd.exe /K %1/mapdata/nodejs/nodevars.bat").arg(configPath);
    m_process->start(exe);
}

void MapView::respProcessState(QProcess::ProcessState state)
{
    qDebug()<<"state:"<<state;
}

void MapView::sreadProcessOutput()
{
    while(!m_process->atEnd()){
        qDebug()<<m_process->readAll();
    }
}

void MapView::respProcessError(QProcess::ProcessError error)
{
    qDebug()<<"error:"<<error;
}

} //namespace MapWeb2D

} //namespace Core
