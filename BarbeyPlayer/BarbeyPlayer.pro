QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

macx {
    QT += openglwidgets opengl
}

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    decoder.cpp \
    display.cpp \
    main.cpp \
    mainwindow.cpp \
    reader.cpp \
    render.cpp

HEADERS += \
    decoder.h \
    display.h \
    mainwindow.h \
    reader.h \
    render.h

FORMS += \
    mainwindow.ui

win32 {
    INCLUDEPATH += "$${PWD}/ThirdParty/ffmpeg/include"

    FFMPEG_LIB_PATH = "$${PWD}/ThirdParty/ffmpeg/lib/win/x64"

    LIBS += "$${FFMPEG_LIB_PATH}/avcodec.lib" \
            "$${FFMPEG_LIB_PATH}/avdevice.lib" \
            "$${FFMPEG_LIB_PATH}/avfilter.lib" \
            "$${FFMPEG_LIB_PATH}/avformat.lib" \
            "$${FFMPEG_LIB_PATH}/avutil.lib" \
            "$${FFMPEG_LIB_PATH}/swresample.lib" \
            "$${FFMPEG_LIB_PATH}/swscale.lib"
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    shader.qrc
