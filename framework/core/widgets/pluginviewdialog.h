/*!
 *  @brief     插件信息视图
 *  @details   以表格的形式显示框架加载的插件信息
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.29
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef PLUGINVIEWDIALOG_H
#define PLUGINVIEWDIALOG_H

#include <QSharedPointer>
#include <QAbstractTableModel>
#include <base/selfwidget/dialogproxy.h>

namespace Core{

class PluginViewDialogPrivate;

class PluginViewDialog : public Base::DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PluginViewDialog)
public:
    explicit PluginViewDialog(QWidget *parent = 0);
    ~PluginViewDialog();

    QSize sizeHint()const;

private slots:
    void respOk();
    void respCancel();
    void showPluginDetail();
    void switchPluginDetail(QModelIndex activeIndex);
    void showErrorInfo();

private:
    QSharedPointer<PluginViewDialogPrivate> d_ptr;
};

class PluginTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PluginTableModel(QObject *parent = 0);
    ~PluginTableModel();

    enum TColumn{
        T_NAME,
        T_ID,
        T_VERSION,
        T_LOADED,
        T_TOTAL_COUNT
    };

    int rowCount(const QModelIndex &parent = QModelIndex())const;
    int columnCount(const QModelIndex &parent = QModelIndex())const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole)const;
    QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QStringList headList;

};

}

#endif // PLUGINVIEWDIALOG_H
