/*
 * libMaia - maiaXmlRpcServerConnection.cpp
 * Copyright (c) 2007 Sebastian Wiedenroth <wiedi@frubar.net>
 *                and Karl Glatz
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// CORE includes
#include <QMetaMethod>
#include <QMetaObject>

// NETWORK includes
#include <QTcpSocket>

// XML includes
#include <QDomDocument>

// maia includes
#include "maiaFault.h"
#include "maiaObject.h"
#include "maiaXmlRpcServer.h"

#include "maiaXmlRpcServerConnection.h"

MaiaXmlRpcServerConnection::MaiaXmlRpcServerConnection( QTcpSocket *connection, QObject* parent )
    : QObject(parent)
{
    mpHeader = NULL;
    mpClientConnection = connection;
    connect(mpClientConnection, SIGNAL(readyRead()), this, SLOT(slReadFromSocket()));
    connect(mpClientConnection, SIGNAL(disconnected()), this, SLOT(deleteLater()));

} // ctor

MaiaXmlRpcServerConnection::~MaiaXmlRpcServerConnection()
{
    mpClientConnection->deleteLater();
    delete mpHeader;

} // dtor

void MaiaXmlRpcServerConnection::slReadFromSocket()
{
    QString lastLine;
    while( mpClientConnection->canReadLine() && !mpHeader ) {
        lastLine = mpClientConnection->readLine();
        mHeaderString += lastLine;
        if( lastLine == "\r\n" ) {
            /* http header end */
            mpHeader = new QHttpRequestHeader(mHeaderString);
            if( !mpHeader->isValid() ) {
                /* return http error */
                qDebug() << "Invalid Header";
                return;
            }
            else if( mpHeader->method() != "POST" ) {
                /* return http error */
                qDebug() << "No Post!";
                return;
            }
            else if( !mpHeader->contentLength() ) {
                /* return fault */
                qDebug() << "No Content Length";
                return;
            }
        }
    }

    if( mpHeader ) {
        if( mpHeader->contentLength() <= mpClientConnection->bytesAvailable() ) {
            /* all data complete */
            parseCall(mpClientConnection->readAll());
        }
    }

} // void slReadFromSocket()

void MaiaXmlRpcServerConnection::sendResponse( const QString &content )
{
    QHttpResponseHeader header(200, "Ok");
    QByteArray block;
    header.setValue("Server", "MaiaXmlRpc/0.1");
    header.setValue("Content-Type", "text/xml");
    header.setValue("Connection","close");
    block.append(header.toString().toUtf8());
    block.append(content.toUtf8());
    mpClientConnection->write(block);
    mpClientConnection->disconnectFromHost();

} // void sendResponse( const QString &content )

void MaiaXmlRpcServerConnection::parseCall( const QString &call )
{
    QDomDocument doc;
    QList<QVariant> args;
    QVariant ret;
    QString response;
    QObject *responseObject;
    const char *responseSlot;

    if( !doc.setContent(call) ) {
        /* recieved invalid xml */
        MaiaFault fault(-32700, tr("Parse error: Not well formed!"));
        sendResponse(fault.toString());
        return;
    }

    QDomElement methodNameElement = doc.documentElement().firstChildElement("methodName");
    QDomElement params = doc.documentElement().firstChildElement("params");
    if( methodNameElement.isNull() ) {
        /* invalid call */
        MaiaFault fault(-32600, tr("Server error: Invalid xml-rpc. Not conforming to spec!"));
        sendResponse(fault.toString());
        return;
    }

    QString methodName = methodNameElement.text();

    emit sgGetMethod(methodName, &responseObject, &responseSlot);
    if( !responseObject ) {
        /* unknown method */
        MaiaFault fault(-32601, tr("Server error: Requested method not found!"));
        sendResponse(fault.toString());
        return;
    }

    QDomNode paramNode = params.firstChild();
    while( !paramNode.isNull() ) {
        args << MaiaObject::fromXml(paramNode.firstChild().toElement());
        paramNode = paramNode.nextSibling();
    }

    if( !invokeMethodWithVariants(responseObject, responseSlot, args, &ret) ) {
        /* error invoking... */
        MaiaFault fault(-32602, tr("Server error: Invalid method parameters!"));
        sendResponse(fault.toString());
        return;
    }

    if( ret.canConvert<MaiaFault>() ) {
        response = ret.value<MaiaFault>().toString();
    }
    else {
        response = MaiaObject::prepareResponse(ret);
    }
    sendResponse(response);

} // void parseCall( const QString &call )

/*
taken from http://delta.affinix.com/2006/08/14/invokemethodwithvariants/
thanks to Justin Karneges once again :)
*/
bool MaiaXmlRpcServerConnection::invokeMethodWithVariants( QObject *obj,
                                                           const QByteArray &method,
                                                           const QVariantList &args,
                                                           QVariant *ret,
                                                           Qt::ConnectionType type )
{
    // QMetaObject::invokeMethod() has a 10 argument maximum
    if( args.count() > 10 ) {
        return false;
    }

    QList<QByteArray> argTypes;
    for( int n = 0; n < args.count(); ++n ) {
        argTypes += args[n].typeName();
    }

    // get return type
    int metatype = 0;
    QByteArray retTypeName = getReturnType(obj->metaObject(), method, argTypes);
    if( !retTypeName.isEmpty() && retTypeName != "QVariant" ) {
        metatype = QMetaType::type(retTypeName.data());
        if( metatype == 0 ) {
            // lookup failed
            return false;
        }
    }

    QGenericArgument arg[10];
    for( int n = 0; n < args.count(); ++n ) {
        arg[n] = QGenericArgument(args[n].typeName(), args[n].constData());
    }

    QGenericReturnArgument retarg;
    QVariant retval;
    if( metatype != 0 && retTypeName != "void" ) {
        retval = QVariant(metatype, (const void *)0);
        retarg = QGenericReturnArgument(retval.typeName(), retval.data());
    }
    else {
        /* QVariant */
        retarg = QGenericReturnArgument("QVariant", &retval);
    }

    if( retTypeName.isEmpty() || retTypeName == "void" ) {
        /* void */
        if( !QMetaObject::invokeMethod(obj, method.data(), type,
                                       arg[0], arg[1], arg[2], arg[3], arg[4],
                                       arg[5], arg[6], arg[7], arg[8], arg[9]) ) {
            return false;
        }
    }
    else {
        if( !QMetaObject::invokeMethod(obj, method.data(), type, retarg,
                                       arg[0], arg[1], arg[2], arg[3], arg[4],
                                       arg[5], arg[6], arg[7], arg[8], arg[9]) ) {
            return false;
        }
    }

    if( retval.isValid() && ret ) {
        *ret = retval;
    }
    return true;

} // bool invokeMethodWithVariants( QObject *obj, const QByteArray &method, const QVariantList &args, QVariant *ret, Qt::ConnectionType type )

QByteArray MaiaXmlRpcServerConnection::getReturnType( const QMetaObject *obj,
                                                      const QByteArray &method,
                                                      const QList<QByteArray> &argTypes )
{
    for( int n = 0; n < obj->methodCount(); ++n ) {
        QMetaMethod m = obj->method(n);
#if QT_VERSION >= 0x050000
        if( m.name() == method && m.parameterTypes() == argTypes ) {
            return m.typeName();
        }
#else
        QByteArray sig = m.signature();

        int offset = sig.indexOf('(');
        if( offset == -1 ) {
            continue;
        }

        QByteArray name = sig.mid(0, offset);
        if( name == method && m.parameterTypes() == argTypes ) {
            return m.typeName();
        }
#endif
    }
    return QByteArray();

} // QByteArray getReturnType( const QMetaObject *obj, const QByteArray &method, const QList<QByteArray> &argTypes )

#if QT_VERSION >= 0x050000
/*
  simple Qt4 class emulater
*/
QHttpRequestHeader::QHttpRequestHeader( const QString &headerString )
{
    mHeaderString = headerString;

    QStringList hdrs = headerString.split("\r\n");
    QStringList hdrkv;
    for( int i = 0; i < hdrs.size(); ++i ) {
        if( hdrs.at(i).trimmed().isEmpty() ) {
            break;
        }

        if( i == 0 ) {
            hdrkv = hdrs.at(i).split(" ");
            mMethod = hdrkv.at(0);
        }
        else {
            hdrkv = hdrs.at(i).split(":");
            mHeaders[hdrkv.at(0)] = hdrkv.at(1).trimmed();
        }
    }

} // ctor

bool QHttpRequestHeader::isValid() const
{
    if( mHeaderString.isEmpty() ) {
        return false;
    }

    if( mMethod != "GET" && mMethod != "POST" ) {
        return false;
    }

    if( mHeaders.size() < 2 ) {
        return false;
    }
    return true;

} // bool isValid() const

QString QHttpRequestHeader::method() const
{
    return mMethod;

} // QString method() const

uint QHttpRequestHeader::contentLength() const
{
    uint clen = mHeaders.value("Content-Length", "0").toUInt();
    return clen;

} // uint contentLength() const

QHttpResponseHeader::QHttpResponseHeader( int code, const QString &text )
{
    mCode = code;
    mText = text;

} // ctor

void QHttpResponseHeader::setValue( const QString &key, const QString &value )
{
    mHeaders[key] = value;

} // void setValue( const QString &key, const QString &value )

QString QHttpResponseHeader::toString() const
{
    QMapIterator<QString, QString> it(mHeaders);
    QString hdrstr;

    hdrstr += QString("HTTP/1.1 %1 %2\r\n").arg(mCode).arg(mText);
    while( it.hasNext() ) {
        it.next();
        hdrstr += it.key() + ": " + it.value() + "\r\n";
    }
    hdrstr += "\r\n";

    return hdrstr;

} // QString toString() const

#endif
