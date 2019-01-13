QT += charts

CONFIG += c++11

HEADERS += \
    chart.h \
    chartview.h \
    TBezierInterpolation.h

SOURCES += \
    chart.cpp \
    chartview.cpp \
    main.cpp


target.path = $$[QT_INSTALL_EXAMPLES]/charts/chartinteractions
INSTALLS += target
