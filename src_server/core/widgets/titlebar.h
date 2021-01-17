/*!
 *  @brief     标题栏显示区
 *  @details   标题栏下部分为三块区域
 *             | 绘制区 | 自定义显示区 | 菜单区 |
 *             绘制区的宽度占屏幕宽度的30%,自定义显示区宽度占比32%,菜单区宽度占比38%，可由SubWidgetGeometryInfo信息定义
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.14
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QPushButton>
#include <QIcon>
#include <core/selfwidget/widget.h>

class QPaintEvent;

namespace Core{

class IconButton : public QPushButton{

    Q_OBJECT
public:
    explicit IconButton(QWidget * parent = 0 );
    ~IconButton();

    void setText(QString text);
    QString text(){return this->m_content;}

    void setPixmap(QPixmap pix);
    QPixmap pixmap(){return this->m_pix;}

private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent * event);
    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);
    void resizeEvent(QResizeEvent * event);

private:
    QString m_content;
    QPixmap m_pix;
    bool mouseHover;
    bool mousePressed;
    QFont textFont;
};

class TitleBarPrivate;

class TitleBar : public Widget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TitleBar)
public:
    explicit TitleBar(QWidget * parent = 0);
    ~TitleBar();

    void addMenu(QString name,QString id,QPixmap buttPix);

    void setTitle(QString title);
    QString title()const;

    void setIcon(QPixmap pixmap);

public:
    /*!
     *  @brief 子部件区域占比信息
     */
    struct SubWidgetGeometryInfo{
        SubWidgetGeometryInfo():m_d_painting(0.3),m_d_toolArea(0.32),m_d_menuArea(0.38){}
        double m_d_painting;
        double m_d_toolArea;
        double m_d_menuArea;
    };

public slots:
    void showTrackingPos(bool enableView, QString viewContent);

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void resizeEvent(QResizeEvent *);
    void updateFeatures();

private:
    void animationView(bool isVisible);

private:
    TitleBarPrivate * d_ptr;
};

}

#endif // TITLEBAR_H
