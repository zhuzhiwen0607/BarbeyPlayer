#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
//#include <QLayout>
#include "render.h"
#include "decoder.h"
#include "displaywidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:


public slots:
    void OnFileOpen();
    void OnAppExit();
    void OnPlay();
    void OnPause();
    void OnStop();
    void OnChangeVolume();
    void OnChangeProgress();

protected:
    virtual void paintEvent(QPaintEvent *event) override;

    void InitUI();
//    void CreateDisplayWidget();

private:
    Ui::MainWindow *ui;

    DisplayWidget *m_pDisplayWidget = nullptr;



    QMenuBar *m_pMenuBar = nullptr;
    QMenu *m_pFileMenu = nullptr;
    QPushButton *m_pPlayButton = nullptr;
    QPushButton *m_pPauseButton = nullptr;
    QPushButton *m_pStopButton = nullptr;
    QSlider *m_pVolumeSlider = nullptr;
    QSlider *m_pProgressSlider = nullptr;

    Decoder *m_pDecoder = nullptr;
    Reader *m_pReader = nullptr;
    Render *m_pRender = nullptr;

};
#endif // MAINWINDOW_H
