#include "reader.h"
#include <QDebug>

Reader::Reader()
{

}

Reader::~Reader()
{
    quit();
    wait();

    m_file.close();

    for (int i = 0; i < m_freePixBlocks.size(); ++i)
    {
        PixBlock *pixblock = m_freePixBlocks[i];
        delete [] pixblock->data;
    }
    m_freePixBlocks.clear();

    for (int i = 0; i < m_filledPixBlocks.size(); ++i)
    {
        PixBlock *pixblock = m_filledPixBlocks[i];
        delete [] pixblock->data;
    }
    m_filledPixBlocks.clear();

}

bool Reader::Initialize(Config &config)
{
    moveToThread(this);

    m_config = config;

    InitFreePixBlocks();

    if (!m_config.fileName.isEmpty())
    {
        OpenFile(m_config.fileName);

        qInfo() << "Reader::Initialize: open file" << m_config.fileName;
    }

    start();

    return true;

}

void Reader::OpenFile(QString &fileName)
{
    m_file.setFileName(m_config.fileName);
//    m_file.open(QIODevice::ReadOnly);

}

void Reader::Play()
{
    if (!m_file.fileName().isEmpty() && !m_file.isOpen())
    {
        m_file.open(QIODevice::ReadOnly);
    }

    m_bPlay = true;
}

void Reader::Pause()
{
    m_bPlay = false;
}

void Reader::Stop()
{
    m_file.close();
}

void Reader::run()
{
    while (true)
    {
        ReadFile();

        msleep(16);     // 60fps
    }
}

void Reader::InitFreePixBlocks(int blocknum)
{
    const int blocksize = m_config.blocksize;

    for (int i = 0; i < blocknum; ++i)
    {
        PixBlock *pixblock = new PixBlock;
        pixblock->size = blocksize;
        pixblock->data = new char [blocksize];
        memset(pixblock->data, 0, blocksize);

        m_freePixBlocks.append(pixblock);
    }
}

Reader::PixBlock* Reader::GetFreePixBlock()
{
    QMutexLocker locker(&m_pixBlocksMutex);
    if (m_freePixBlocks.isEmpty())
        return nullptr;

    return m_freePixBlocks.front();
}

void Reader::AddPixBlock(PixBlock *pixblock)
{
    if (!pixblock)
        return;

    QMutexLocker locker(&m_pixBlocksMutex);
    m_filledPixBlocks.append(pixblock);
    m_freePixBlocks.removeOne(pixblock);

}

void Reader::FreePixBlock(PixBlock *pixblock)
{
    if (!pixblock)
        return;

    QMutexLocker locker(&m_pixBlocksMutex);
    m_filledPixBlocks.removeOne(pixblock);
    m_freePixBlocks.append(pixblock);
    memset(pixblock->data, 0, m_config.blocksize);
}

Reader::PixBlock* Reader::GetFilledPixBlock()
{
    QMutexLocker locker(&m_pixBlocksMutex);
    if (m_filledPixBlocks.isEmpty())
        return nullptr;

    return m_filledPixBlocks.front();
}


void Reader::ReadFile()
{
    if (!m_bPlay)
    {
        return;
    }

    if (!m_file.isOpen())
    {
        qInfo() << "Reader::ReadFile: file is not open";
        return;
    }

//    qInfo() << "Reader::ReadFile";

    const int blocksize = m_config.blocksize;
    PixBlock *pixblock = GetFreePixBlock();
    if (!pixblock)
        return;

    char *data = pixblock->data;

    qint64 readedbytes = m_file.read(data, blocksize);
    if (readedbytes <= 0)
    {
        // end of file or read failed, stop program

        return;
    }

    pixblock->size = readedbytes;
    AddPixBlock(pixblock);

    emit sigNewFrame();

}
