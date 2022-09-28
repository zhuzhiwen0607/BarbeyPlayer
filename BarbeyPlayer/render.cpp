#include "render.h"

#include <QTime>

static QTime g_time;


#define VERTEX_POS 0
#define VERTEX_TEX 1
/*
const char *vsrc =
        "attribute vec4 vertexIn; \
         attribute vec4 textureIn; \
         varying vec4 textureOut;  \
         void main(void)           \
         {                         \
             gl_Position = vertexIn; \
             textureOut = textureIn; \
         }";

const char *fsrc =
         "varying mediump vec4 textureOut;\n"
         "uniform sampler2D textureY;\n"
         "uniform sampler2D textureUV;\n"
         "void main(void)\n"
         "{\n"
         "vec3 yuv; \n"
         "vec3 rgb; \n"
         "yuv.x = texture2D(textureY, textureOut.st).r; \n"
         "yuv.y = texture2D(textureUV, textureOut.st).r - 0.5; \n"
         "yuv.z = texture2D(textureUV, textureOut.st).g - 0.5; \n"
         "rgb = mat3( 1.164,       1.164,         1.164, \n"
                     "0.0,       -0.392,  2.017, \n"
                     "1.596, -0.813,  0.0) * yuv; \n"
         "gl_FragColor = vec4(rgb, 1); \n"
         "}\n";
*/

Render::Render(QWidget *parent) : QOpenGLWidget(parent)
{
    memset(&m_config, 0, sizeof(m_config));
    m_pReader = nullptr;
    m_pDecoder = nullptr;

    g_time.start();
}

Render::~Render()
{
    if (m_pReader)
        disconnect(m_pReader, &Reader::sigNewFrame, this, &Render::OnNewFrame);

    if (m_pDecoder)
        disconnect(m_pDecoder, &Decoder::sigNewFrame, this, &Render::OnNewFrame);
}

bool Render::Initialize(Config &config, Reader *reader)
{
    m_config = config;
    m_pReader = reader;
    memset(m_vbo, 0, sizeof(m_vbo));
    memset(m_samples, 0, sizeof(m_samples));
    memset(m_textures, 0, sizeof(m_textures));

//    connect(m_pReader, &Reader::sigNewFrame, this, &Render::OnNewFrame);

    qInfo() << "finish Render::Initialize";

    return true;
}

bool Render::Initialize(Config &config, Decoder *decoder)
{
    if (!decoder)
    {
        qWarning() << "Render::Initialize: decoder is null";
        return false;
    }

    m_config = config;
//    m_pReader = reader;
    m_pDecoder = decoder;
    memset(m_vbo, 0, sizeof(m_vbo));
    memset(m_samples, 0, sizeof(m_samples));
    memset(m_textures, 0, sizeof(m_textures));

//    connect(m_pReader, &Reader::sigNewFrame, this, &Render::OnNewFrame);
    connect(decoder, &Decoder::sigNewFrame, this, &Render::OnNewFrame);

    qInfo() << "finish Render::Initialize";

    return true;
}

void Render::PixImageTest()
{
    QImage image("D:\\Projects\\testmedia\\ironman.PNG");

//    QPainter painter(this);

    qInfo() << QString::asprintf("render size(%d, %d)", width(), height());

}

void Render::OnNewFrame()
{
    update();
}

void Render::initializeGL()
{
    qInfo() << "initializeGL";

    initializeOpenGLFunctions();

    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh");

//    m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
//    m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);

    m_shaderProgram.bindAttributeLocation("ipos", VERTEX_POS);
    m_shaderProgram.bindAttributeLocation("itex", VERTEX_TEX);
    m_shaderProgram.link();
    m_shaderProgram.bind();

    static const float vertices[] = {
        -1.0f, -1.0f,   // left bottom
        +1.0f, -1.0f,   // right bottom
        -1.0f, +1.0f,   // left top
        +1.0f, +1.0f,   // right top
    };

    static const float textures[] = {
        0.0f, 1.0f,     // left top
        1.0f, 1.0f,     // right top
        0.0f, 0.0f,     // left bottom
        1.0f, 0.0f,     // right bottom
    };

    glGenBuffers(2, m_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    glVertexAttribPointer(VERTEX_POS, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(VERTEX_POS);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textures), textures, GL_STREAM_DRAW);
    glVertexAttribPointer(VERTEX_TEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(VERTEX_TEX);

    m_samples[0] = m_shaderProgram.uniformLocation("textureY");
    m_samples[1] = m_shaderProgram.uniformLocation("textureUV");
//    m_samples[2] = m_shaderProgram.uniformLocation("textureV");

    glGenTextures(2, m_textures);

    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_config.width, m_config.height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, m_config.width >> 1, m_config.height >> 1, 0, GL_RG, GL_UNSIGNED_BYTE, nullptr);

//    glBindTexture(GL_TEXTURE_2D, m_textures[2]);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_config.width >> 1, m_config.height >> 1, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

}

void Render::paintGL()
{

//    qInfo() << "Render::paintGL";

//    Reader::PixBlock *pixblock = m_pReader->GetFilledPixBlock();    // NV12
//    if (!pixblock)
//    {
//        qInfo() << "Render::paintGL: GetFilledPixBlock return null";
//        return;
//    }
    AVFrame *pFrame = m_pDecoder->GetFilledVideoFrame();
    if (!pFrame)
    {
//        qInfo() << "paintGL: m_pDecoder->GetFilledVideoFrame return null";
        return;
    }

    qint64 t1 = g_time.elapsed();

    static QByteArray data;
    int width = pFrame->width;
    int height = pFrame->height;
    int size = av_image_get_buffer_size((AVPixelFormat)pFrame->format, width, height, 1);
    data.resize(size);
    av_image_copy_to_buffer((uint8_t*)data.data(), size, pFrame->data, pFrame->linesize, (AVPixelFormat)pFrame->format, width, height, 1);

//    const char *y = pixblock->data;
//    const char *uv = y + m_config.width * m_config.height;
//    int width = pFrame->width;
//    int height = pFrame->height;


//    const char *y = pFrame->data;
//    const char *uv = pFrame->data + width * height;

//    const int sizeY = m_config.width * m_config.height;
//    const int sizeUV = m_config.width * m_config.height / 2;


//    char *dataY = new char[sizeY];
//    memset(dataY, 0, sizeY);
//    memcpy(dataY, y, sizeY);

//    char *dataUV = new char[sizeUV];
//    memset(dataUV, 0, sizeUV);
//    memcpy(dataUV, uv, sizeUV);

//    qInfo() << QString::asprintf("paintGL: width=%d, height=%d", m_config.width, m_config.height);

//    glUseProgram(m_shaderProgram);

    char *dataY = data.data();
    char *dataUV = data.data() + width * height;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, dataY);
    glUniform1i(m_samples[0], 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width >> 1, height >> 1, GL_RG, GL_UNSIGNED_BYTE, dataUV);
    glUniform1i(m_samples[1], 1);

//    glActiveTexture(GL_TEXTURE2);
//    glBindTexture(GL_TEXTURE_2D, m_textures[2]);
//    glTexSubImage2D(GL_TEXTURE2, 0, 0, 0, m_config.width >> 1, m_config.height >> 1, GL_RED, GL_UNSIGNED_BYTE, v);
//    glUniform1i(m_samples[2], 2);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//    m_pReader->FreePixBlock(pixblock);
    m_pDecoder->FreeVideoFrame(pFrame);

    qint64 t2 = g_time.elapsed();

    qInfo() << "paintGL: interval t =" << (t2 - t1);

}

/*
void Render::paintGL()
{

//    qInfo() << "Render::paintGL";

    Reader::PixBlock *pixblock = m_pReader->GetFilledPixBlock();    // NV12
    if (!pixblock)
    {
        qInfo() << "Render::paintGL: GetFilledPixBlock return null";
        return;
    }

    const char *y = pixblock->data;
    const char *uv = y + m_config.width * m_config.height;
//    const char *v = u + m_config.width * m_config.height;

    const int sizeY = m_config.width * m_config.height;
    const int sizeUV = m_config.width * m_config.height / 2;
    char *dataY = new char[sizeY];
    memset(dataY, 0, sizeY);
    memcpy(dataY, y, sizeY);

    char *dataUV = new char[sizeUV];
    memset(dataUV, 0, sizeUV);
    memcpy(dataUV, uv, sizeUV);


    #if 0
    static bool writeflag = false;
    if (!writeflag)
    {
        QFile outFile("D:\\Projects\\testmedia\\out.yuv");
        outFile.open(QIODevice::WriteOnly);

        outFile.write(y, m_config.width * m_config.height);
        outFile.write(u, m_config.width * m_config.height / 2);

        writeflag = true;
    }
    #endif

//    qInfo() << QString::asprintf("paintGL: width=%d, height=%d", m_config.width, m_config.height);

//    glUseProgram(m_shaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_config.width, m_config.height, GL_RED, GL_UNSIGNED_BYTE, dataY);
    glUniform1i(m_samples[0], 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_config.width >> 1, m_config.height >> 1, GL_RG, GL_UNSIGNED_BYTE, dataUV);
    glUniform1i(m_samples[1], 1);

//    glActiveTexture(GL_TEXTURE2);
//    glBindTexture(GL_TEXTURE_2D, m_textures[2]);
//    glTexSubImage2D(GL_TEXTURE2, 0, 0, 0, m_config.width >> 1, m_config.height >> 1, GL_RED, GL_UNSIGNED_BYTE, v);
//    glUniform1i(m_samples[2], 2);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_pReader->FreePixBlock(pixblock);

}
*/
void Render::resizeGL(int w, int h)
{

}
