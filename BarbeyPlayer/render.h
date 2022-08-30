#ifndef RENDER_H
#define RENDER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include "reader.h"

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
    explicit Render();
    ~Render();
    bool Initialize(Config &config, Reader *reader = nullptr);




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

    QOpenGLShaderProgram m_shaderProgram;
    GLuint m_vbo[2];
    GLuint m_textures[2];

};

#endif // RENDER_H
