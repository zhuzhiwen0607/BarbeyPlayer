#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
//#include <QSlider>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);



//    m_pDisplayWidget = new DisplayWidget(this);


    m_pReader = new Reader;
    Reader::Config readerConfig = { };
    readerConfig.blocksize = 1920 * 1080 * 3 / 2;
//    readerConfig.fileName = "D:\\Projects\\testmedia\\Bumblebee-20s.yuv";
    m_pReader->Initialize(readerConfig);

//    m_pRender = new Render(m_pDisplayWidget);
    m_pRender = new Render(this);
    Render::Config renderConfig = { };
    renderConfig.width = 1920;
    renderConfig.height = 1080;
    m_pRender->Initialize(renderConfig, m_pReader);

    setCentralWidget(m_pRender);
//    m_pRender->raise();
//    m_pRender->show();


//    m_pReader->Play();


    InitUI();

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

void MainWindow::OnPlay()
{
    m_pReader->Play();
}

void MainWindow::OnPause()
{
    m_pReader->Pause();
}

void MainWindow::OnStop()
{
    m_pReader->Stop();
}

void MainWindow::OnChangeVolume()
{

}

void MainWindow::OnChangeProgress()
{

}

void MainWindow::paintEvent(QPaintEvent *event)
{
//    QPainter painter(this);
//    painter.setPen(Qt::blue);
//    painter.setFont(QFont("Arial", 30));
//    painter.drawText(rect(), Qt::AlignCenter, "Qt");
}

void MainWindow::InitUI()
{
    // menu
    m_pFileMenu = menuBar()->addMenu(tr("&File"));
    m_pFileMenu->addAction(tr("&Open"), this, &MainWindow::OnFileOpen);
    m_pFileMenu->addAction(tr("&Exit"), this, &MainWindow::OnAppExit);

    // button
    m_pPlayButton = new QPushButton("Play");
    m_pPlayButton->setMaximumWidth(80);
    connect(m_pPlayButton, &QPushButton::clicked, this, &MainWindow::OnPlay);

    m_pPauseButton = new QPushButton("Pause");
    m_pPauseButton->setMaximumWidth(80);
    connect(m_pPauseButton, &QPushButton::clicked, this, &MainWindow::OnPause);

    m_pStopButton = new QPushButton("Stop");
    m_pStopButton->setMaximumWidth(80);
    connect(m_pStopButton, &QPushButton::clicked, this, &MainWindow::OnStop);

    m_pVolumeSlider = new QSlider(Qt::Vertical);
    m_pVolumeSlider->setMaximumHeight(50);
    connect(m_pVolumeSlider, &QSlider::sliderMoved, this, &MainWindow::OnChangeVolume);

    m_pProgressSlider = new QSlider(Qt::Horizontal);
    m_pProgressSlider->setMaximumWidth(10000);
//    m_pProgressSlider->setMaximum(1000);
//    m_pProgressSlider->setMinimumSize();
    connect(m_pProgressSlider, &QSlider::sliderMoved, this, &MainWindow::OnChangeProgress);

//    m_pRender->setAutoFillBackground(true);
//    QPalette plt = palette();
//    plt.setColor(QPalette::Window, Qt::black);
//    m_pRender->setPalette(plt);

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(m_pPlayButton);
    layout1->addWidget(m_pPauseButton);
    layout1->addWidget(m_pStopButton);
    layout1->addWidget(m_pVolumeSlider);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addWidget(m_pRender);
    layout2->addWidget(m_pProgressSlider);
    layout2->addLayout(layout1);

    QWidget *centerWidget = new QWidget;
    centerWidget->setLayout(layout2);
    setCentralWidget(centerWidget);

}


