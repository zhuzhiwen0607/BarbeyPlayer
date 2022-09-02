#ifndef DECODER_H
#define DECODER_H

#include <QThread>

class Decoder : public QThread
{
    Q_OBJECT

public:
    explicit Decoder();
    ~Decoder();

    bool Initialize();
};

#endif // DECODER_H
