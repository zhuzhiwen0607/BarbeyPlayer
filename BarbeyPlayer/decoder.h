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
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
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
        AVMediaType mediaType;
        AVCodecContext *decCtx;
//        AVPacket *decPacket;
        AVFrame *frame;
    };

public:
    explicit Decoder();
    ~Decoder();

    bool Initialize(Config &config);

    AVFrame* GetFreeVideoFrame();
    AVFrame* GetFreeAudioFrame();
    AVFrame* GetFilledVideoFrame();
    AVFrame* GetFilledAudioFrame();
    void FillVideoFrame(AVFrame* frame);
    void FillAudioFrame(AVFrame* frame);
    void FreeVideoFrame(AVFrame* frame);
    void FreeAudioFrame(AVFrame* frame);

signals:
    void sigNewFrame();

public slots:
    void OnOpen(QString);
    void OnStart();
    void OnStop();
    void OnPlay();
    void OnPause();

protected:
    virtual void run() override;
    void MainDecode();
    void DecodeVideo(StreamContext *pStreamContext);
    void DecodeAudio(StreamContext *pStreamContext);

    void InitFreeVideoFrames(const int n);
    void InitFreeAudioFrames(const int n);

private:
    Config m_config = { };
    bool m_runPlay = false;

    // video parameters
    int m_videoWidth;
    int m_videoHeight;
    AVPixelFormat m_videoFormat;
    SwsContext *m_pSwsContext = nullptr;

    AVFormatContext *m_pFormatContext = nullptr;
    StreamContext m_streamContexts[4];
    AVPacket *m_pPacket = nullptr;

    QMutex m_videoMutex;
    QVector<AVFrame*> m_freeVideoFrames;
    QVector<AVFrame*> m_filledVideoFrames;

    QMutex m_audioMutex;
    QVector<AVFrame*> m_freeAudioFrames;
    QVector<AVFrame*> m_filledAudioFrames;

};

#endif // DECODER_H
