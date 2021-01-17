#include "stylemanager.h"

#include <QFile>
#include <QDir>
#include <QApplication>
#include <QDebug>

#include "../util/rlog.h"
#include "../util/rutil.h"

namespace Base{

class StyleManagerPrivate
{
    Q_DECLARE_PUBLIC(StyleManager)

public:
    StyleManagerPrivate(StyleManager * q):q_ptr(q){

    }

    StyleManager * q_ptr;
    StylePtrList slist;
    CustomStyle * currStyle;
};

StyleManager::StyleManager():d_ptr(new StyleManagerPrivate(this)),QObject()
{

}

void StyleManager::addStyle(CustomStyle *style)
{
    Q_D(StyleManager);
    d->slist.push_back(style);
}

/*!
 * @brief 从指定的路径中加载动态的样式属性
 * @param[in] stylePath 待加载的样式路径
 */
void StyleManager::dynamicLoadStyle(QString stylePath)
{
    if(stylePath.isEmpty())
        return;

    if(!RUtil::isDir(stylePath))
        return;

    QDir styleDir(stylePath);
    QFileInfoList styleInfoList = styleDir.entryInfoList(QDir::Dirs);
    std::for_each(styleInfoList.begin(),styleInfoList.end(),[&](QFileInfo & fileInfo){
        if(isValidStyleDir(fileInfo.absoluteFilePath())){
            addStyle(new CustomStyle(fileInfo.fileName(),getStyleFile(fileInfo.absoluteFilePath()),false));
        }
    });
}

int StyleManager::size()
{
    Q_D(StyleManager);
    return d->slist.size();
}

StylePtrList StyleManager::styles()
{
    Q_D(StyleManager);
    return d->slist;
}

void StyleManager::switchStyle(int index)
{
    Q_D(StyleManager);
    Q_ASSERT(index < d->slist.size());
    d->currStyle = d->slist.at(index);
    switch(d->currStyle->getStyleType()){
        case CustomStyle::STYLE_SHEET:
            {
                QFile styleFile(d->currStyle->getStylePath());
                if(!styleFile.open(QFile::ReadOnly)){
                    RLOG_ERROR("style file %s read error!",d->currStyle->getStylePath().toLocal8Bit().data());
                    return;
                }
                qApp->setStyleSheet(styleFile.readAll());
            }
            break;
        case CustomStyle::STYLE_CUSTOMSTYLE:
            switch(d->currStyle->getClazz()){
                //qApp->setStyle();
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

CustomStyle * StyleManager::currentStyle()
{
    Q_D(StyleManager);
    return d->currStyle;
}

CustomStyle *StyleManager::findStyle(QString styleName)
{
    Q_D(StyleManager);
    for(int i = 0; i < d->slist.size();i++){
        if(d->slist.at(i)->getStyleName() == styleName)
            return d->slist.at(i);
    }
    return NULL;
}

/*!
 * @brief 检测传递的路径是否符合标准的样式结构
 * @warning 目录结构必须严格遵守以下路径格式：
 *          [+]dark:
 *                [+]img:【可选】
 *                [+]qss:【必须】
 *                   [-]style.qss:【必须】
 * @param[in] singleStyleDir 待检测的样式路径
 * @return true：符合标准
 */
bool StyleManager::isValidStyleDir(QString singleStyleDir)
{
    QFileInfo styleInfo(getStyleFile(singleStyleDir));
    return styleInfo.exists();
}

QString StyleManager::getStyleFile(QString styleRootPath)
{
    return QString(styleRootPath + QDir::separator()+ QString("qss/style.qss"));
}

}
