/*!
 *  @brief     样式管理器
 *  @details   提供对内置样式的管理，支持从指定配置目录下读取扩展的样式文件
 *             样式分为内置和动态加载两种方式获取：
 *                   1.内置：以qrc资源文件形式编译至程序内部，若动态加载失败时，以内置的主题为默认主题；
 *                   2.动态加载：以主题为单位存放于config/schemes目录下，子目录包括img、css等文件夹；
 *             不管是内置还是动态加载的形式，均会在[设置]-[样式]菜单中创建触发按钮。
 *
 *             以dark目录为例：
 *             [+]dark:主题名
 *                [+]img：存放当前主题所需要的图片信息【可选】；
 *                [+]qss：存放主题的qss信息【必须】；
 *                   [-]style.qss：主题样式文件，系统会根据此名称加载主题，找不到此文件即失败【必须】；
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.15
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note 2018.12.13:wey:增加从置顶路径下加载样式文件;
 */
#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QObject>
#include <QList>

#include "../base_global.h"

class QAction;

namespace Base{

/*!
 *  @brief 自定义样式信息描述
 *  @details
 */
class BASESHARED_EXPORT CustomStyle{

public:
    /*!
     *  @brief 自定义样式类枚举
     *  @details 每添加一种style，即添加一个字段
     */
    enum CustomStyleClass{

    };

    CustomStyle(){}

    CustomStyle(QString name,QString path,bool selected = false):name(name),path(path),selected(selected),stype(STYLE_SHEET){

    }

    CustomStyle(QString name,CustomStyleClass clazz,bool selected = false):name(name),clazz(clazz),selected(selected),stype(STYLE_CUSTOMSTYLE){

    }

    enum StyleType{
        STYLE_SHEET,            /*!< 样式表文件 */
        STYLE_CUSTOMSTYLE       /*!< 自定义样式类 */
    };

    QString getStyleName()const{return this->name;}
    StyleType getStyleType(){return this->stype;}
    CustomStyleClass getClazz(){return this->clazz;}
    QString getStylePath()const{return this->path;}
    bool isSelected()const{return this->selected;}

    void setAction( QAction * action){this->action = action;}
     QAction * getAction(){return this->action;}

private:
    QString name;           /*!< 样式名 */
    StyleType stype;        /*!< 样式类型 */
    CustomStyleClass clazz; /*!< 自定义样式类型 */
    QString path;           /*!< 样式保存路径，可为本地路径，也可为资源文件中路径 */
    bool selected;          /*!< 是否被选中显示 */
    QAction * action;       /*!< 工具栏对应按钮 */
};

typedef QList<CustomStyle *> StylePtrList;
class  StyleManagerPrivate;

class BASESHARED_EXPORT StyleManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(StyleManager)
public:
    explicit StyleManager();

    void addStyle(CustomStyle * style);
    void dynamicLoadStyle(QString stylePath);

    int size();
    StylePtrList styles();
    void switchStyle(int index);
    CustomStyle *currentStyle();
    CustomStyle * findStyle(QString styleName);

private:
    bool isValidStyleDir(QString singleStyleDir);
    inline QString getStyleFile(QString styleRootPath);

private:
    StyleManagerPrivate * d_ptr;
};

}

#endif // STYLEMANAGER_H
