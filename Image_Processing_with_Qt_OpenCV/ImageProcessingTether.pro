QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageProcessingTether
TEMPLATE = app

INCLUDEPATH += ./include \
                /usr/local/include/

VPATH += include\
         src\
         images\
        resources\

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        serialhandling.cpp

HEADERS += \
        mainwindow.h \
        serialhandling.h

FORMS += \
        include/mainwindow.ui

RESOURCES += resources.qrc

DESTDIR = ../bin

LIBS += `pkg-config --libs opencv`
