TARGET = maia
TEMPLATE = lib

CONFIG += staticlib

QT += xml network
QT -= gui

HEADERS += \
    maiaFault.h \
    maiaObject.h \
    maiaXmlRpcClient.h \
    maiaXmlRpcServer.h \
    maiaXmlRpcServerConnection.h

SOURCES += \
    maiaFault.cpp \
    maiaObject.cpp \
    maiaXmlRpcClient.cpp \
    maiaXmlRpcServer.cpp \
    maiaXmlRpcServerConnection.cpp

# Installation
MAIA_INSTALL_DIR = $$PREFIX

isEmpty(MAIA_INSTALL_DIR) {
    unix {
        MAIA_INSTALL_DIR = /usr/local
    }

    win32 {
        MAIA_INSTALL_DIR = C:/maia
    }
}

target.path = $$MAIA_INSTALL_DIR/lib

headers.files += $$HEADERS
headers.path = $$MAIA_INSTALL_DIR/include/maia

INSTALLS += \
    target \
    headers
