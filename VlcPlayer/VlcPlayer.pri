include($$PWD/../dependencies.pri)
INCLUDEPATH += $${VLC_INCLUDE_PATH}
LIBS+= -L$${VLC_LIB_PATH} -l$$qtLibraryTarget(tiff)
