QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    src/CNNRecognizer.cpp \
    src/FastDeskew.cpp \
    src/FineMapping.cpp \
    src/Pipeline.cpp \
    src/PlateDetection.cpp \
    src/PlateSegmentation.cpp \
    src/Recognizer.cpp \
    src/util.cpp \
    src/niblackthreshold.cpp \
    src/PlateInfo.cpp

HEADERS += \
    include/CNNRecognizer.h \
    include/FastDeskew.h \
    include/FineMapping.h \
    include/niBlackThreshold.h \
    include/Pipeline.h \
    include/PlateDetection.h \
    include/PlateInfo.h \
    include/PlateSegmentation.h \
    include/Recognizer.h \
    include/util.h \
    include/base.h

win32 {
    LIBS += ..\libs\win\libopencv_*.a
}

DESTDIR += ./
