#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QLayout>
#include "controlpanel.h"

class DisplayWidget : public QWidget
{
    Q_OBJECT

public:



    explicit DisplayWidget(QWidget *parent = nullptr);
    ~DisplayWidget();

signals:

public slots:

protected:


private:
    QLayout *m_pLayout = nullptr;
//    QSlider *m_pVideoSlider = nullptr;
    ControlPanel *m_pControlPanel = nullptr;

};

#endif // DISPLAYWIDGET_H
