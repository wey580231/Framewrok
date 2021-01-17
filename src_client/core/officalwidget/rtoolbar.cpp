#include "rtoolbar.h"

namespace Core{

RToolBar * RToolBar::m_instance = nullptr;

RToolBar *RToolBar::instance()
{
    if(m_instance == nullptr)
        m_instance = new RToolBar();

    return m_instance;
}

RToolBar::~RToolBar()
{

}

RToolBar::RToolBar(QWidget *parent):QToolBar(parent)
{

}

} //namespace Core
