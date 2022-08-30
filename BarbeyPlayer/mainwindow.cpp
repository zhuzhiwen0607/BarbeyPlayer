#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);


    CreateMenus();


    m_pReader = new Reader;
    Reader::Config readerConfig = { };
    readerConfig.blocksize = 1920 * 1080 * 4;
    readerConfig.fileName = "D:\\Projects\\testmedia\\Bumblebee-20s.yuv";
    m_pReader->Initialize(readerConfig);

    m_pRender = new Render;
    Render::Config renderConfig = { };
    renderConfig.width = 1920;
    renderConfig.height = 1080;
    m_pRender->Initialize(renderConfig, m_pReader);



    setCentralWidget(m_pRender);


    QLayout layout;



//    m_pReader->Play();


//        QSlider


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnFileOpen()
{
    qDebug() << "MainWindow::OnFileOpen";

//    QFileDialog fileDialog(this);
//    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
//    fileDialog.setWindowTitle(tr("Open Media File"));

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Media File"), "", tr("YUV Files (*.yuv)"));

    if (fileName.isEmpty())
        return;

    m_pReader->OpenFile(fileName);

}

void MainWindow::OnAppExit()
{
    exit(0);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
//    QPainter painter(this);
//    painter.setPen(Qt::blue);
//    painter.setFont(QFont("Arial", 30));
//    painter.drawText(rect(), Qt::AlignCenter, "Qt");
}

void MainWindow::CreateMenus()
{
    m_pFileMenu = menuBar()->addMenu(tr("&File"));

    m_pFileMenu->addAction(tr("&Open"), this, &MainWindow::OnFileOpen);
    m_pFileMenu->addAction(tr("&Exit"), this, &MainWindow::OnAppExit);

}


