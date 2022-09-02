#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


//    Reader *m_pReader = new Reader;
//    Reader::Config readerConfig = { };
//    readerConfig.blocksize = 1920 * 1080 * 3 / 2;
//    readerConfig.fileName = "D:\\Projects\\testmedia\\Bumblebee-20s.yuv";
//    m_pReader->Initialize(readerConfig);

//    Render *m_pRender = new Render();
//    Render::Config renderConfig = { };
//    renderConfig.width = 1920;
//    renderConfig.height = 1080;
//    m_pRender->Initialize(renderConfig, m_pReader);

//    m_pRender->show();

//    m_pReader->Play();


    return a.exec();
}
