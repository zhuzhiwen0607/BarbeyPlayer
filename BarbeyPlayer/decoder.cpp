#include "decoder.h"
#include <QDebug>

Decoder::Decoder()
{

}

Decoder::~Decoder()
{

}

bool Decoder::Initialize(Config &config)
{
    m_config = config;
    int ret;

    moveToThread(this);

    const char *inputFileName = config.filename.toStdString().c_str();
    ret = avformat_open_input(&m_pFormatContext, inputFileName, NULL, NULL);
    if (ret < 0)
    {
        qWarning() << "avformat_open_input error, ret =" << ret;
        return false;
    }

    ret = avformat_find_stream_info(m_pFormatContext, NULL);
    if (ret < 0)
    {
        qWarning() << "avformat_find_stream_info error, ret =" << ret;
        return false;
    }

//    const int nStreams = m_pFormatContext->nb_streams;
//    m_pStreamContext = new StreamContext[nStreams];
    m_pStreamContext = (StreamContext*)av_calloc(m_pFormatContext->nb_streams, sizeof(StreamContext));

    for (int i = 0; i < m_pFormatContext->nb_streams; ++i)
    {
        int ret;
        AVStream *pStream = m_pFormatContext->streams[i];

        const AVCodec *pCodec = avcodec_find_decoder(pStream->codecpar->codec_id);

        AVCodecContext *pCodecContext = avcodec_alloc_context3(pCodec);
        if (!pCodec)
        {
            qWarning() << QString::asprintf("streams[%d]: avcodec_alloc_context3 error", i);
            return false;
        }

        ret = avcodec_parameters_to_context(pCodecContext, pStream->codecpar);
        if (ret < 0)
        {
            qWarning() << QString::asprintf("streams[%d]: avcodec_parameters_to_context error, %d", i, ret);
            return false;
        }

        qDebug() << QString::asprintf("streams[%d]: codecType=%d", pCodecContext->codec_type);

        ret = avcodec_open2(pCodecContext, pCodec, NULL);
        if (ret < 0)
        {
            qWarning() << QString::asprintf("streams[%d]: avcodec_open2 error, %d", i, ret);
            return false;
        }

        m_pStreamContext[i].decCtx = pCodecContext;
//        m_pStreamContext[i].decPacket = av_packet_alloc();
        m_pStreamContext[i].decFrame = av_frame_alloc();

    }

    m_pPacket = av_packet_alloc();

    av_dump_format(m_pFormatContext, 0, inputFileName, 0);

    start();

    return true;
}

void Decoder::OnStart()
{

}

void Decoder::OnStop()
{

}

void Decoder::OnPlay()
{

}

void Decoder::OnPause()
{

}

void Decoder::run()
{
    while (true)
    {
        DecodeLoop();
        msleep(10);
    }
}

void Decoder::DecodeLoop()
{
    int ret;

    ret = av_read_frame(m_pFormatContext, m_pPacket);
    if (ret < 0)
    {
        qWarning() << "av_read_frame error, ret =" << ret;
        return;
    }

    const int streamIdx = m_pPacket->stream_index;
    StreamContext *pStreamContext = &m_pStreamContext[streamIdx];

    ret = avcodec_send_packet(pStreamContext->decCtx, m_pPacket);
    if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
    {
        qWarning() << "avcodec_send_packet error, ret =" << ret;
        return;
    }

    do
    {
        ret = avcodec_receive_frame(pStreamContext->decCtx, pStreamContext->decFrame);
        if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
        {
            break;
        }
        else if (ret < 0)
        {
            // avcodec_receive_frame error, should exit
            qWarning() << "avcodec_receive_frame error, ret =" << ret;
            return;
        }
        else
        {
            // avcodec_receive_frame success, store the frame to buffer to render and wave

            qDebug() << QString::asprintf("avcodec_receive_frame: ");
        }
    } while (ret >= 0);

}

void Decoder::InitFreeVideoFrames(const int n)
{
    for (int i = 0; i < n; ++i)
    {
        AVFrame *frame = av_frame_alloc();
    }
}

void Decoder::InitFreeAudioFrames(const int n)
{

}

