#include "render.h"

#define VERTEX_POS 0
#define VERTEX_TEX 1

Render::Render()
{
    memset(&m_config, 0, sizeof(m_config));
    m_pReader = nullptr;
}

Render::~Render()
{
    disconnect(m_pReader, &Reader::sigNewFrame, this, &Render::OnNewFrame);
}

bool Render::Initialize(Config &config, Reader *reader)
{
    m_config = config;
    m_pReader = reader;
    memset(m_vbo, 0, sizeof(m_vbo));
    memset(m_textures, 0, sizeof(m_textures));

    connect(m_pReader, &Reader::sigNewFrame, this, &Render::OnNewFrame);

    return true;
}

void Render::OnNewFrame()
{
    update();
}

void Render::initializeGL()
{
    initializeOpenGLFunctions();

    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh");
    m_shaderProgram.link();
    m_shaderProgram.bind();

    float vertices[] = {
        -1.0f, -1.0f,   // left bottom
        +1.0f, -1.0f,   // right bottom
        +1.0f, +1.0f,   // right top
        -1.0f, +1.0f,   // left top
    };

    float textures[] = {
        0.0f, 0.0f,     // left bottom
        1.0f, 0.0f,     // right bottom
        1.0f, 1.0f,     // right top
        0.0f, 1.0f,     // left top
    };

    glGenBuffers(2, m_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textures), textures, GL_STREAM_DRAW);

    glVertexAttribPointer(VERTEX_POS, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(VERTEX_POS);
    glVertexAttribPointer(VERTEX_TEX, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(VERTEX_TEX);


    glGenTextures(2, m_textures);

    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_config.width, m_config.height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, m_config.width >> 1, m_config.height >> 1, 0, GL_RG, GL_UNSIGNED_BYTE, nullptr);

}

void Render::paintGL()
{

    Reader::PixBlock *pixblock = m_pReader->GetFilledPixBlock();    // NV12
    if (!pixblock)
        return;

    const char *y = pixblock->data;
    const char *uv = pixblock->data + m_config.width * m_config.height;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glTexSubImage2D(GL_TEXTURE0, 0, 0, 0, m_config.width, m_config.height, GL_RED, GL_UNSIGNED_BYTE, y);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    glTexSubImage2D(GL_TEXTURE1, 0, 0, 0, m_config.width >> 1, m_config.height >> 1, GL_RG, GL_UNSIGNED_BYTE, uv);

    glDrawArrays(GL_TRIANGLES, 0, 4);
}

void Render::resizeGL(int w, int h)
{

}
