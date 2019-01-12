QT      += core widgets

CONFIG  += plugin

TARGET = $$qtLibraryTarget(vlcplayer)
TEMPLATE = lib

include($$PWD/../dependencies.pri)
include($$PWD/../Global.pri)
include($$PWD/../OpenCV.pri)
DESTDIR = $${BINPATH}/plugins/videoshowrecord

HEADERS += \
    IMediaPlayer.h \
    IMediaPlayerPlugin.h \
    MediaPlayer.h \
    MediaPlayerPlugin.h \
    VlcCameraWriter.h \
    VlcVideoPlayer.h

SOURCES += \
    MediaPlayer.cpp \
    MediaPlayerPlugin.cpp \
    VlcCameraWriter.cpp \
    VlcVideoPlayer.cpp

INCLUDEPATH += $$VLC_INCLUDE_PATH
LIBS += -L$$VLC_LIB_PATH
win32 {
    !contains(QMAKE_TARGET.arch, x86_64) {
        LIBS += -llibvlc
    } else {
        LIBS += -llibvlc.x64
    }
}

DISTFILES += \
    MediaPlayerPlugin.json \
    vlcplayer.ini
