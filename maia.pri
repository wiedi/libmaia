QT += xml network
QT -= gui

INCLUDEPATH += \
    $$PWD

DEPENDPATH += \
    $$PWD

HEADERS += \
    $$PWD/maiaFault.h \
    $$PWD/maiaObject.h \
    $$PWD/maiaXmlRpcClient.h \
    $$PWD/maiaXmlRpcServer.h \
    $$PWD/maiaXmlRpcServerConnection.h

SOURCES += \
    $$PWD/maiaFault.cpp \
    $$PWD/maiaObject.cpp \
    $$PWD/maiaXmlRpcClient.cpp \
    $$PWD/maiaXmlRpcServer.cpp \
    $$PWD/maiaXmlRpcServerConnection.cpp
