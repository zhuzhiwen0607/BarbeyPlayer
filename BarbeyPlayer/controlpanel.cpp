#include "controlpanel.h"

ControlPanel::ControlPanel(QWidget *parent) : QWidget(parent)
{
    m_pLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(m_pLayout);

    m_pVideoSlider = new QSlider(Qt::Horizontal, this);
    m_pLayout->addWidget(m_pVideoSlider);

}

ControlPanel::~ControlPanel()
{

}
