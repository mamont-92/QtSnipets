QT      += core widgets

CONFIG  += plugin

TARGET = $$qtLibraryTarget(mediaplayerplugin)
TEMPLATE = lib

include(dependencies.pri)

#VLC
INCLUDEPATH += $$VLC_INCLUDE_PATH

LIBS += -L$${VLC_LIB_PATH}
LIBS += -l$$qtLibraryTarget(libvlc)

#OpenCV
INCLUDEPATH += $$OPENCV_INCLUDE_PATH

LIBS += -L$${OPENCV_LIB_PATH}

LIBS += -l$$qtLibraryTarget(opencv_calib3d310) \
 -l$$qtLibraryTarget(opencv_core310) \
 -l$$qtLibraryTarget(opencv_features2d310) \
 -l$$qtLibraryTarget(opencv_flann310) \
 -l$$qtLibraryTarget(opencv_highgui310) \
 -l$$qtLibraryTarget(opencv_imgcodecs310) \
 -l$$qtLibraryTarget(opencv_imgproc310) \
 -l$$qtLibraryTarget(opencv_ml310) \
 -l$$qtLibraryTarget(opencv_objdetect310) \
 -l$$qtLibraryTarget(opencv_photo310) \
 -l$$qtLibraryTarget(opencv_shape310) \
 -l$$qtLibraryTarget(opencv_stitching310) \
 -l$$qtLibraryTarget(opencv_superres310) \
 -l$$qtLibraryTarget(opencv_video310) \
 -l$$qtLibraryTarget(opencv_videoio310) \
 -l$$qtLibraryTarget(opencv_videostab310)


SOURCES += VLC_CameraWriter.cpp \
    MediaPlayer.cpp \
    VLC_VideoPlayer.cpp \
    MediaPlayerPlugin.cpp

HEADERS  += VLC_CameraWriter.h \
    MediaPlayer.h \
    VLC_VideoPlayer.h \
    IMediaPlayer.h \
    MediaPlayerPlugin.h \
	IMediaPlayerPlugin.h

DISTFILES += \
    MediaPlayerPlugin.json
