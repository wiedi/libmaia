######################################################################
# Automatically generated by qmake (2.01a) Fr Mai 25 19:04:58 2007
######################################################################

TARGET = server
TEMPLATE = app

QT += xml network
QT -= gui

INCLUDEPATH += \
    ../../src

DEPENDPATH += \
    ../../src

win32 {
    # show console window
    CONFIG += console

    CONFIG(debug,debug|release) {
        LIBS += -L../../src/debug -lmaia
    }
    else {
        LIBS += -L../../src/release -lmaia
    }
}

unix {
    LIBS += -L../../src -lmaia
}

HEADERS += \
    server.h

SOURCES += \
    server.cpp \
    server_main.cpp
