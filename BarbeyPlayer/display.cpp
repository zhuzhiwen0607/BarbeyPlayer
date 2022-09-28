#include "display.h"

Display::Display()
{

}

Display::~Display()
{

}

//bool Display::Initialize(Decoder *decoder)
bool Display::Initialize(Render *render)
{
//    if (!decoder)
//        return;
    if (!render)
        return false;

    m_pRender = render;

    moveToThread(this);

//    m_pRender = new Render(this);
//    Render::Config renderConfig = { };
//    renderConfig.width = 1920;
//    renderConfig.height = 1080;
//    m_pRender->Initialize(renderConfig, decoder);


    start();

    return true;
}

void Display::run()
{
    while (true)
    {
//        m_pRender->OnNewFrame();
//        emit sigNewFrame();
        m_pRender->OnNewFrame();
        msleep(10);
    }
}
