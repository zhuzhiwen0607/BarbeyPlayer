#include "decoder.h"
#include <QDebug>

Decoder::Decoder()
{
    memset(m_streamContexts, 0, sizeof(m_streamContexts));
    m_pFormatContext = nullptr;
    m_videoFormat = AV_PIX_FMT_NV12;
}

Decoder::~Decoder()
{

}

bool Decoder::Initialize(Config &config)
{
    m_config = config;


    moveToThread(this);

    start();

    return true;
}

//AVFrame* GetFreeVideoFrame();
//AVFrame* GetFreeAUdioFrame();
//AVFrame* GetFilledVideoFrame();
//AVFrame* GetFilledAudioFrame();
//void FillVideoFrame(const AVFrame* frame);
//void FillAudioFrame(const AVFrame* frame);
//void FreeVideoFrame(const AVFrame* frame);
//void FreeAudioFrame(const AVFrame* frame);

AVFrame* Decoder::GetFreeVideoFrame()
{
    QMutexLocker locker(&m_videoMutex);

    if (m_freeVideoFrames.isEmpty())
        return nullptr;

    return m_freeVideoFrames.front();
}

void Decoder::FillVideoFrame(AVFrame *frame)
{
    QMutexLocker locker(&m_videoMutex);

    m_freeVideoFrames.removeOne(frame);
    m_filledVideoFrames.append(frame);
}

AVFrame* Decoder::GetFilledVideoFrame()
{
    QMutexLocker locker(&m_videoMutex);

    if (m_filledVideoFrames.isEmpty())
        return nullptr;

    return m_filledVideoFrames.front();
}

void Decoder::FreeVideoFrame(AVFrame *frame)
{
    if (!frame)
        return;

    QMutexLocker locker(&m_videoMutex);

    m_filledVideoFrames.removeOne(frame);
    m_freeVideoFrames.append(frame);
}

/*
AVFrame* Decoder::GetFilledFrame()
{
    QMutexLocker locker(&m_videoMutex);

    if (m_filledVideoFrames.isEmpty())
    {
        return nullptr;
    }
    return m_filledVideoFrames.front();
}

AVFrame* Decoder::GetFreeFrame()
{
    QMutexLocker locker(&m_videoMutex);

    if (m_freeVideoFrames.isEmpty())
    {
        return nullptr;
    }
    return m_freeVideoFrames.front();
}

void Decoder::FillFrame(const AVFrame *frame)
{
    if (!frame)
    {
        return;
    }

    QMutexLocker locker(&m_videoMutex);

    m_freeVideoFrames.removeOne(frame);
    m_filledVideoFrames.append(frame);
}

void Decoder::FreeFrame(const AVFrame *frame)
{
    if (!frame)
    {
        return;
    }

    QMutexLocker locker(&m_videoMutex);

    m_filledVideoFrames.removeOne(frame);
    m_freeVideoFrames.append(frame);
}
*/
void Decoder::OnOpen(QString filename)
{
    int ret = 0;

//    const char *inputFileName = filename.toStdString().c_str();
//    const char *inputFileName = "E:\\videos\\demo_1080p.mp4";
    QByteArray inputFileName = filename.toUtf8();

    qInfo() << "Decoder::OnOpen: inputFileName =" << inputFileName.data();
    ret = avformat_open_input(&m_pFormatContext, inputFileName.data(), NULL, NULL);
    if (ret < 0)
    {
        qWarning() << "avformat_open_input error, ret =" << ret;
        return;
    }

    ret = avformat_find_stream_info(m_pFormatContext, NULL);
    if (ret < 0)
    {
        qWarning() << "avformat_find_stream_info error, ret =" << ret;
        return;
    }

    for (int i = 0; i < m_pFormatContext->nb_streams; ++i)
    {
        int ret;
        AVStream *pStream = m_pFormatContext->streams[i];

        const AVCodec *pCodec = avcodec_find_decoder(pStream->codecpar->codec_id);

        AVCodecContext *pCodecContext = avcodec_alloc_context3(pCodec);
        if (!pCodec)
        {
            qWarning() << QString::asprintf("streams[%d]: avcodec_alloc_context3 error", i);
            return;
        }

        ret = avcodec_parameters_to_context(pCodecContext, pStream->codecpar);
        if (ret < 0)
        {
            qWarning() << QString::asprintf("streams[%d]: avcodec_parameters_to_context error, %d", i, ret);
            return;
        }

        qDebug() << QString::asprintf("streams[%d]: codecType=%d", pCodecContext->codec_type);

        ret = avcodec_open2(pCodecContext, pCodec, NULL);
        if (ret < 0)
        {
            qWarning() << QString::asprintf("streams[%d]: avcodec_open2 error, %d", i, ret);
            return;
        }

        m_streamContexts[i].mediaType = pStream->codecpar->codec_type;
        m_streamContexts[i].decCtx = pCodecContext;
        m_streamContexts[i].frame = av_frame_alloc();

        AVMediaType mediaType = pStream->codecpar->codec_type;
        if (AVMEDIA_TYPE_VIDEO == mediaType)
        {
            m_videoWidth = pStream->codecpar->width;
            m_videoHeight = pStream->codecpar->height;

            int srcW = m_videoWidth;
            int srcH = m_videoHeight;
            int dstW = m_videoWidth;
            int dstH = m_videoHeight;
            AVPixelFormat srcFormat = (AVPixelFormat)pStream->codecpar->format;
            AVPixelFormat dstFormat = m_videoFormat;

            m_pSwsContext = sws_getContext(srcW, srcH, srcFormat,
                                           dstW, dstH, dstFormat,
                                           SWS_BILINEAR, NULL, NULL, NULL);
        }
        else if (AVMEDIA_TYPE_AUDIO == mediaType)
        {

        }
        else
            ;

    }


    m_pPacket = av_packet_alloc();

    av_dump_format(m_pFormatContext, 0, inputFileName, 0);


    InitFreeVideoFrames(m_config.videoFrameNums);

    InitFreeAudioFrames(m_config.audioFrameNums);

//    qInfo() << QString::asprintf("Decoder::OnOpen: finish open %s", filename.toUtf8());
    qInfo() << "Decoder::OnOpen: finish open" << filename.toUtf8();

    OnPlay();

}

void Decoder::OnStart()
{

}

void Decoder::OnStop()
{

}

void Decoder::OnPlay()
{
    m_runPlay = true;
}

void Decoder::OnPause()
{
    m_runPlay = false;
}

void Decoder::run()
{
    while (true)
    {
        if (m_runPlay)
            MainDecode();

        msleep(10);
    }
}

void Decoder::MainDecode()
{
    int ret = 0;

    ret = av_read_frame(m_pFormatContext, m_pPacket);
    if (ret == 0)
    {
        const int idx = m_pPacket->stream_index;
        StreamContext *pStreamContext = &m_streamContexts[idx];

        ret = avcodec_send_packet(pStreamContext->decCtx, m_pPacket);
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
        {
            qWarning() << "avcodec_send_packet error, ret =" << ret;
            return;
        }
    }

    for (int i = 0; i < m_pFormatContext->nb_streams; ++i)
    {
        StreamContext *pStreamContext = &m_streamContexts[i];

        if (pStreamContext->mediaType == AVMEDIA_TYPE_VIDEO)
        {
            DecodeVideo(pStreamContext);
        }
        else if (pStreamContext->mediaType == AVMEDIA_TYPE_AUDIO)
        {
            DecodeAudio(pStreamContext);
        }
        else
            ;

    }

}

void Decoder::DecodeVideo(StreamContext *pStreamContext)
{
    if (!pStreamContext)
        return;

    AVFrame *pFrame = GetFreeVideoFrame();
    if (!pFrame)
        return;

    int ret = avcodec_receive_frame(pStreamContext->decCtx, pStreamContext->frame);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
    {
        return;
    }

    if (ret < 0 /*&& ret != AVERROR(EAGAIN) && ret != AVERROR_EOF*/)
    {
        qWarning() << "Decoder::DecodeVideo: avcodec_receive_frame error, ret =" << ret;
        exit(0);
    }

    if (pStreamContext->frame->format == pFrame->format)
    {
        ret = av_frame_copy(pFrame, pStreamContext->frame);
        if (ret < 0)
        {
            qWarning() << "Decoder::DecodeVideo: av_frame_copy error, ret =" << ret;
            return;
        }
    }
    else
    {

        sws_scale(m_pSwsContext, pStreamContext->frame->data, pStreamContext->frame->linesize, 0, pStreamContext->frame->height, pFrame->data, pFrame->linesize);
    }


    FillVideoFrame(pFrame);

//    emit sigNewFrame();

}

void Decoder::DecodeAudio(StreamContext *pStreamContext)
{
    if (!pStreamContext)
        return;

    int ret = avcodec_receive_frame(pStreamContext->decCtx, pStreamContext->frame);
    if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
    {
        qWarning() << "avcodec_receive_frame error, ret =" << ret;
        exit(0);
    }
}

void Decoder::InitFreeVideoFrames(const int n)
{
    for (int i = 0; i < n; ++i)
    {
        AVFrame *frame = av_frame_alloc();
        frame->width = m_videoWidth;
        frame->height = m_videoHeight;
        frame->format = m_videoFormat;
        av_frame_get_buffer(frame, 0);
        m_freeVideoFrames.append(frame);
    }
}

void Decoder::InitFreeAudioFrames(const int n)
{

}

