#ifndef DECODER_H
#define DECODER_H

#include <QThread>
#include <QVector>
#include <QMutex>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/opt.h"
}

class Decoder : public QThread
{
    Q_OBJECT
public:
    struct Config
    {
        QString filename;
        int videoFrameNums;
        int audioFrameNums;
    };

    struct StreamContext
    {
        AVCodecContext *decCtx;
//        AVPacket *decPacket;
        AVFrame *decFrame;
    };

public:
    explicit Decoder();
    ~Decoder();

    bool Initialize(Config &config);

signals:

public slots:
    void OnOpen(QString);
    void OnStart();
    void OnStop();
    void OnPlay();
    void OnPause();

protected:
    virtual void run() override;
    void DecodeLoop();

    void InitFreeVideoFrames(const int n);
    void InitFreeAudioFrames(const int n);

private:
    Config m_config = { };
    bool m_runPlay = false;

    AVFormatContext *m_pFormatContext = nullptr;
    StreamContext *m_pStreamContext = nullptr;
    AVPacket *m_pPacket = nullptr;

    QMutex m_videoMutex;
    QVector<AVFrame*> m_freeVideoFrames;

    QMutex m_audioMutex;
    QVector<AVFrame*> m_freeAudioFrames;


};

#endif // DECODER_H
