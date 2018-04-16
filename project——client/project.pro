#-------------------------------------------------
#
# Project created by QtCreator 2018-04-09T09:26:51
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    zhuce.cpp \
    information.cpp \
    tishi.cpp

HEADERS  += widget.h \
    zhuce.h \
    information.h \
    tishi.h

FORMS    += widget.ui \
    zhuce.ui \
    information.ui \
    tishi.ui

RESOURCES += \
    beijing.qrc
