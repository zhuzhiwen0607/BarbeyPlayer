#ifndef RENDER_H
#define RENDER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include "reader.h"
#include "decoder.h"

class Render : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    struct Config
    {
        qint32 width;
        qint32 height;
    };


public:
    explicit Render(QWidget *parent = nullptr);
    ~Render();
    bool Initialize(Config &config, Reader *reader);
    bool Initialize(Config &config, Decoder *decoder);

    void PixImageTest();


signals:

public slots:
    void OnNewFrame();

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

private:

    Config m_config;

    Reader *m_pReader;
    Decoder *m_pDecoder;

    QOpenGLShaderProgram m_shaderProgram;
    GLuint m_vbo[2];
    GLuint m_samples[2];    // nv12
    GLuint m_textures[2];

};

#endif // RENDER_H
