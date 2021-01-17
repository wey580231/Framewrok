#include "officestylewindow.h"

#include <QVBoxLayout>

#include "officestylemenu/officemenubar.h"
#include "mainwindow.h"

namespace Core{

OfficeStyleWindow * OfficeStyleWindow::m_instance = nullptr;

OfficeStyleWindow::OfficeStyleWindow(QWidget * parent):QWidget(parent)
{
    m_instance = this;
    init();
}

OfficeStyleWindow*OfficeStyleWindow::instance()
{
    return m_instance;
}

void OfficeStyleWindow::initAll()
{
    m_mainWindow->initAll();
}

void OfficeStyleWindow::displayResize()
{
   m_mainWindow->displayResize();
   showMaximized();
}


void OfficeStyleWindow::addMenu(AbstractOfficeMenuItem *item)
{
    if(item != nullptr){
        m_menuBar->addMenu(item);
    }
}

void OfficeStyleWindow::closeEvent(QCloseEvent* event)
{
    if(m_mainWindow->close())
        event->accept();
    else
        event->ignore();
}

void OfficeStyleWindow::init()
{
    QVBoxLayout * layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    m_menuBar = new OfficeMenuBar();

    m_mainWindow = new MainWindow();

    layout->addWidget(m_menuBar);
    layout->addWidget(m_mainWindow);

    setLayout(layout);
}

} //namespace Core
