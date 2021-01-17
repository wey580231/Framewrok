/*!
 *  @brief     自定义按钮组
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2020.07.30
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef MENUGROUP_H
#define MENUGROUP_H

#include <QAbstractButton>
#include <QMap>

namespace Core{

/*!
*  @brief 菜单显示模式
*/
enum MenuShowModel{
    FLOW_MODEL,         /*!< 悬浮模式 */
    EMBEDD_MODEL        /*!< 嵌入显示模式 */
};


class MenuGroup;

class RMenuButton : public QAbstractButton
{
    Q_OBJECT
public:
    RMenuButton(QWidget *parent = nullptr);

    QSize sizeHint() const;

signals:
    void hideFlowContainer();

protected:
    void paintEvent(QPaintEvent *e) override;
    void enterEvent(QEvent * event) override;
    void leaveEvent(QEvent * event) override;
    void mousePressEvent(QMouseEvent * event) override;
    void mouseDoubleClickEvent(QMouseEvent * event) override;

private:
    QColor m_normalBackgroundColor;
    QColor m_hoverBackgroundColor;
    QColor m_selectedBackgroundColor;

    QColor m_normalColor;
    QColor m_hoverColor;
    QColor m_selectedColor;

    bool m_bMouseIn;

    friend class MenuGroup;
};

class MenuGroup : public QWidget
{
    Q_OBJECT

signals:
    void menuChecked(QWidget *menuItem,bool isChecked);
    void hideFlowContainer();

private:
    explicit MenuGroup(QWidget * parent = nullptr);
    ~MenuGroup(){}

    void addItem(QString name,QWidget * menuItem);
    void cancelCheckedState();

    struct ItemDesc{
        QString itemName;
        QWidget * itemMenu;
    };

private slots:
    void buttChecked(bool checked);

private:
    QMap<RMenuButton *,ItemDesc> m_items;

    friend class OfficeMenuBar;
};

} //namespace Core

#endif // MENUGROUP_H
