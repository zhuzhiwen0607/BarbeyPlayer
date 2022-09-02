#ifndef READER_H
#define READER_H

#include <QThread>
#include <QFile>
#include <QTimer>
#include <QVector>
#include <QMutex>

class Reader : public QThread
{
    Q_OBJECT

public:
    struct Config
    {
//        qint32 width;
//        qint32 height;
        int blocksize;
//        int blocknum;
        QString fileName;

    };

    struct PixBlock
    {
        int size;
        char *data;
    };

    explicit Reader();
    ~Reader();
    bool Initialize(Config &config);

    void OpenFile(QString &fileName);

    PixBlock* GetFreePixBlock();
    void AddPixBlock(PixBlock *pixblock);
    void FreePixBlock(PixBlock *pixblock);
    PixBlock* GetFilledPixBlock();

    void Play();
    void Pause();
    void Stop();
    void Forward();
    void Backward();
    void Seek();


signals:
    void sigNewFrame();

public slots:

protected:
    virtual void run() override;



private:
    void InitFreePixBlocks(int blocknum = 8);


    void ReadFile();


    Config m_config;
    QFile m_file;
    QTimer m_timer;

    QMutex m_pixBlocksMutex;
    QVector<PixBlock*> m_freePixBlocks;
    QVector<PixBlock*> m_filledPixBlocks;

    bool m_bPlay = false;


};

#endif // READER_H
