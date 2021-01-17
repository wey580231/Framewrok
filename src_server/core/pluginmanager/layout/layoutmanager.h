/*!
 *  @brief     布局管理器
 *  @details   1.负责布局的加载或保存; @n
 *  @author    wey
 *  @version   1.0
 *  @date      2018.12.03
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef LAYOUTMANAGER_H_2018_12_03
#define LAYOUTMANAGER_H_2018_12_03

#include <QObject>

namespace Core{

class LayoutManager : public QObject
{
    Q_OBJECT
public:
    explicit LayoutManager(QObject * parent = 0);

    QStringList localLayoutFilenames();

    bool importView(QString fileName);

    bool existed(QString fileName);

    void resotreDefaultLayout();

    QString getCurrentLayout();
    void setCurrentLayout(QString fileName);

public slots:
    void importView();
    void exportView(bool showOperateResult);

private:
    QString m_sz_layoutFileName;
};

}

#endif // LAYOUTMANAGER_H
