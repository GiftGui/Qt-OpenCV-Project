QT       += core gui network

TEMPLATE = app
TARGET = ImageEditor

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 debug_and_release

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    editor_plugin_interface.h \
    mainwindow.h

FORMS +=

DEFINES += QT_DEPRECATED_WARNINGS

include(d:/opencv/opencv.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
