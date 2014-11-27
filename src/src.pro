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
