/*
 * libMaia - maiaXmlRpcClient.h
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

#ifndef MAIAXMLRPCCLIENT_H
#define MAIAXMLRPCCLIENT_H

// CORE includes
#include <QList>
#include <QObject>
#include <QUrl>
#include <QVariant>

// NETWORK includes
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QSslError>

// maia includes
#include "maia_global.h"

// fwd
class QNetworkReply;
class MaiaObject;

class MAIASHARED_EXPORT MaiaXmlRpcClient : public QObject
{
    Q_OBJECT

signals:
    void sgSslErrors( QNetworkReply *reply, const QList<QSslError> &errors );

public:
    MaiaXmlRpcClient( QObject *parent = 0 );
    MaiaXmlRpcClient( const QUrl &url, QObject *parent = 0 );
    MaiaXmlRpcClient( const QUrl &url, const QString &userAgent, QObject *parent = 0 );

    void setUrl( const QUrl &url );

    void setUserAgent( const QString &userAgent );

    void setNetworkProxy( const QNetworkProxy &proxy );

    void setSslConfiguration( const QSslConfiguration &config );
    QSslConfiguration sslConfiguration() const;

    QNetworkReply *call( const QString &method, const QList<QVariant> &args,
                         QObject *responseObject, const char *responseSlot,
                         QObject *faultObject, const char *faultSlot);

private slots:
    void slReplyFinished( QNetworkReply *reply );

private:
    void init();

    QNetworkAccessManager mNam;
    QNetworkRequest mRequest;
    QMap<QNetworkReply *, MaiaObject *> mCallMap;

};

#endif
