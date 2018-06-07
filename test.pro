#-------------------------------------------------
#
# Project created by QtCreator 2016-12-22T10:50:43
#
#-------------------------------------------------

QT       += core gui multimedia
RC_FILE = pic/logo.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#CONFIG += warn_on debug console

TARGET = 五子棋小游戏
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    GameModel.cpp \
    menu.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    GameModel.h \
    menu.h \
    about.h

RESOURCES += \
    resource.qrc

FORMS += \
    menu.ui \
    about.ui

DISTFILES +=

