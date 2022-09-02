#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QBoxLayout>
#include <QSlider>

class ControlPanel : public QWidget
{
    Q_OBJECT

public:

    explicit ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

private:
    QBoxLayout *m_pLayout = nullptr;
    QSlider *m_pVideoSlider = nullptr;
};

#endif // CONTROLPANEL_H
