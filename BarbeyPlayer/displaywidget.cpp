#include "displaywidget.h"
#include <QBoxLayout>

DisplayWidget::DisplayWidget(QWidget *parent) : QWidget(parent)
{
//    m_pLayout = new QBoxLayout(QBoxLayout::BottomToTop, this);
//    setLayout(m_pLayout);

    m_pControlPanel = new ControlPanel(this);
//    m_pLayout->addWidget(m_pControlPanel);

//    m_pControlPanel->move(this->x()+ 5, this->y() + this->height());

    m_pControlPanel->move(parent->x() + 5, parent->y() + parent->height() - 10);




}

DisplayWidget::~DisplayWidget()
{

}

