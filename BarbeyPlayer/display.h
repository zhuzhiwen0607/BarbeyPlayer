#ifndef DISPLAY_H
#define DISPLAY_H

#include <QThread>
#include "render.h"
//#include "decoder.h"

class Display : public QThread
{
    Q_OBJECT

public:
    struct Config
    {

    };

signals:
    void sigNewFrame();

public:

    explicit Display();
    ~Display();
//    bool Initialize(Decoder *decoder);
//    bool Initialize(Render *render);
    bool Initialize();

protected:

    virtual void run() override;

private:
//    Render *m_pRender;
//    Decoder *m_pDecoder;
};

#endif // DISPLAY_H
