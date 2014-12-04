TARGET = maia
TEMPLATE = lib

QT += network xml
QT -= gui

include(build.pri)
maia_static {
    CONFIG += staticlib
}
else {
    CONFIG += shared

    DEFINES += MAIA_BUILD_SHARED
}

HEADERS += \
    maia_global.h \
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
