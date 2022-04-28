QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    capture_thread.cpp \
    main.cpp \
    mainwindow.cpp \
    utilities.cpp

HEADERS += \
    capture_thread.h \
    mainwindow.h \
    utilities.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DEFINES += OPENCV_DATA_DIR=\\\"D:/opencv/build/install/etc/\\\"

win32: {
  include(d:/opencv/opencv.pri)
}
unix: !macx{
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}
unix: macx{
   INCLUDEPATH += "/usr/local/include"
   LIBS += -L"/usr/local/lib" \
   -lopencv_world
}
