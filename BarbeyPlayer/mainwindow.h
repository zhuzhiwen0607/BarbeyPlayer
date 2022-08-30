#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "displaywidget.h"
#include "render.h"

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

protected:
    virtual void paintEvent(QPaintEvent *event) override;

    void CreateMenus();
//    void CreateDisplayWidget();

private:
    Ui::MainWindow *ui;

//    DisplayWidget *m_pDisplayWidget = nullptr;

    QMenuBar *m_pMenuBar = nullptr;
    QMenu *m_pFileMenu = nullptr;

    Reader *m_pReader = nullptr;
    Render *m_pRender = nullptr;

};
#endif // MAINWINDOW_H
