/*
 * libMaia - maiaXmlRpcServerConnection.h
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

#ifndef MAIAXMLRPCSERVERCONNECTION_H
#define MAIAXMLRPCSERVERCONNECTION_H

// CORE includes
#include <QByteArray>
#include <QList>
#include <QObject>
#include <QVariant>

// maia includes
#include "maia_global.h"

#if QT_VERSION < 0x050000

// NETWORK includes
#include <QHttpRequestHeader>
#include <QHttpResponseHeader>

#else
class MAIASHARED_EXPORT QHttpRequestHeader
{
public:
    explicit QHttpRequestHeader( const QString &headerString );
    virtual ~QHttpRequestHeader() {}

    bool isValid() const;
    QString method() const;
    uint contentLength() const;

private:
    QString mHeaderString;
    QString mMethod;
    QMap<QString, QString> mHeaders;

};

class MAIASHARED_EXPORT QHttpResponseHeader
{
public:
    explicit QHttpResponseHeader( int code, const QString &text );
    virtual ~QHttpResponseHeader() {}

    void setValue( const QString &key, const QString &value );
    virtual QString toString() const;

private:
    int mCode;
    QString mText;
    QMap<QString, QString> mHeaders;

};
#endif

// fwd
class QTcpSocket;

class MAIASHARED_EXPORT MaiaXmlRpcServerConnection : public QObject
{
    Q_OBJECT

signals:
    void sgGetMethod( const QString &method, QObject **responseObject, const char **responseSlot );

public:
    MaiaXmlRpcServerConnection( QTcpSocket *connection, QObject *parent = 0 );
    ~MaiaXmlRpcServerConnection();

private slots:
    void slReadFromSocket();

private:
    void sendResponse( const QString &content );
    void parseCall( const QString &call );
    bool invokeMethodWithVariants( QObject *obj, const QByteArray &method, const QVariantList &args, QVariant *ret, Qt::ConnectionType type = Qt::AutoConnection );

    static QByteArray getReturnType( const QMetaObject *obj, const QByteArray &method, const QList<QByteArray> &argTypes );

    QTcpSocket *mpClientConnection;
    QHttpRequestHeader *mpHeader;
    QString mHeaderString;

};

#endif
