#include "systemsetting.h"

#include <QGridLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>
#include <QMap>
#include <QListWidgetItem>
#include <QSplitter>

#include <base/util/rutil.h>
#include "abstractsettingpage.h"

namespace Core {

typedef QMap<QString,AbstractSettingPage *> PageMapping;

class SystemSettingPrivate
{
    Q_DECLARE_PUBLIC(SystemSetting)
public:
    SystemSettingPrivate(SystemSetting * q):q_ptr(q){
        init();
    }

    void init();
    void addPage(AbstractSettingPage * page);

    QLineEdit * pageFilterEdit;
    QListWidget * listWidget;
    QLabel * currPageNameLabel;
    QStackedWidget * pageStack;

    PageMapping pageMappings;

    SystemSetting * q_ptr;
};

void SystemSettingPrivate::init()
{
    QWidget * mainWidget = new QWidget(q_ptr);

    pageFilterEdit = new QLineEdit(mainWidget);

    listWidget = new QListWidget(mainWidget);
    listWidget->setFocusPolicy(Qt::NoFocus);
    listWidget->setObjectName("systemSetting_list");
    listWidget->setFixedWidth(120);

    currPageNameLabel = new QLabel(mainWidget);
    currPageNameLabel->setAlignment(Qt::AlignCenter);
    currPageNameLabel->setStyleSheet("font-weight:600;");

    pageStack = new QStackedWidget(mainWidget);

    QObject::connect(listWidget,SIGNAL(currentRowChanged(int)),q_ptr,SLOT(switchPage(int)));

    QGridLayout * mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(4,4,4,4);
    mainLayout->setHorizontalSpacing(8);
    mainLayout->addWidget(pageFilterEdit,0,0,1,1);
    mainLayout->addWidget(currPageNameLabel,0,1,1,1);
    mainLayout->addWidget(listWidget,1,0,1,1);
    mainLayout->addWidget(pageStack,1,1,1,1);

    mainWidget->setLayout(mainLayout);

    q_ptr->setContentWidget(mainWidget);
}

/*!
 * @brief 添加新的设置页面
 * @param[in]   page 新配置页面
 */
void SystemSettingPrivate::addPage(AbstractSettingPage *page)
{
    listWidget->addItem(page->pageName());
    pageStack->addWidget(page->pageWidget());
}

SystemSetting::SystemSetting(QWidget *parent):Base::DialogProxy(parent),d_ptr(new SystemSettingPrivate(this))
{
    setTitle(QStringLiteral("系统设置"));
    setButton(Base::DialogProxy::Save,this,SLOT(respOk()));
    setButton(Base::DialogProxy::Close,this,SLOT(respCancel()));

    setAttribute(Qt::WA_DeleteOnClose);
}

SystemSetting::~SystemSetting()
{

}

QSize SystemSetting::sizeHint() const
{
    QRect screenSize = Base::RUtil::screenGeometry();
    return QSize(screenSize.width() * 0.4,screenSize.height() * 0.5);
}

void SystemSetting::addPage(AbstractSettingPage *page)
{
    Q_D(SystemSetting);
    if(page == NULL || page->pageWidget() == NULL || page->pageName().isEmpty())
        return;

    if(!d->pageMappings.contains(page->pageName())){
        d->pageMappings.insert(page->pageName(),page);
        d->addPage(page);
        d->listWidget->setCurrentRow(0);
    }
}

void SystemSetting::respOk()
{
    Q_D(SystemSetting);
    PageMapping::iterator iter = d->pageMappings.begin();
    while(iter != d->pageMappings.end()){
        if(iter.value()->respOk())
            iter++;
        else
            return;
    }

    respCancel();
}

void SystemSetting::respCancel()
{
    Q_D(SystemSetting);
    close();
}

/*!
 * @brief 设置页面切换，更新页面信息
 * @param[in]   index 待更新的索引
 */
void SystemSetting::switchPage(int index)
{
    Q_D(SystemSetting);
    if(index >= 0 && index < d->pageMappings.size()){
        QString pageName = d->listWidget->currentItem()->text();
        AbstractSettingPage * page = d->pageMappings.value(pageName);
        if(page){
            d->currPageNameLabel->setText(page->pageName());
            d->pageStack->setCurrentIndex(index);
        }
    }
}

} //namespace Core

