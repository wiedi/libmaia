/*
 * libMaia - maiaXmlRpcClient.cpp
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

// NETWORK includes
#include <QNetworkReply>

// maia includes
#include "maiaFault.h"
#include "maiaObject.h"

#include "maiaXmlRpcClient.h"

MaiaXmlRpcClient::MaiaXmlRpcClient( QObject *parent )
    : QObject(parent), mNam(this), mRequest()
{
    init();

} // ctor

MaiaXmlRpcClient::MaiaXmlRpcClient( const QUrl &url, QObject *parent )
    : QObject(parent), mNam(this), mRequest(url)
{
    init();
    setUrl(url);

} // ctor

MaiaXmlRpcClient::MaiaXmlRpcClient( const QUrl &url, const QString &userAgent, QObject *parent )
    : QObject(parent)
{
    // userAgent should adhere to RFC 1945 http://tools.ietf.org/html/rfc1945
    init();
    mRequest.setRawHeader("User-Agent", userAgent.toLatin1());
    setUrl(url);

} // ctor

void MaiaXmlRpcClient::setUrl( const QUrl &url )
{
    if( !url.isValid() ) {
        return;
    }
    mRequest.setUrl(url);

} // void setUrl( const QUrl &url )

void MaiaXmlRpcClient::setUserAgent( const QString &userAgent )
{
    mRequest.setRawHeader("User-Agent", userAgent.toLatin1());

} // void setUserAgent( const QString &userAgent )

void MaiaXmlRpcClient::setSslConfiguration( const QSslConfiguration &config )
{
    mRequest.setSslConfiguration(config);

} // void setSslConfiguration( const QSslConfiguration &config )

QSslConfiguration MaiaXmlRpcClient::sslConfiguration() const
{
    return mRequest.sslConfiguration();

} // QSslConfiguration sslConfiguration() const

QNetworkReply *MaiaXmlRpcClient::call( const QString &method, const QList<QVariant> &args,
                                       QObject *responseObject, const char *responseSlot,
                                       QObject *faultObject, const char *faultSlot )
{
    MaiaObject *call = new MaiaObject(this);
    connect(call, SIGNAL(sgResponse(QVariant&,QNetworkReply*)), responseObject, responseSlot);
    connect(call, SIGNAL(sgFault(int,QString,QNetworkReply*)), faultObject, faultSlot);

    QNetworkReply *reply = mNam.post(mRequest, MaiaObject::prepareCall(method, args).toUtf8());

    mCallMap[reply] = call;
    return reply;

} // QNetworkReply *call( const QString &method, const QList<QVariant> &args, QObject *responseObject, const char *responseSlot, QObject *faultObject, const char *faultSlot)

void MaiaXmlRpcClient::slReplyFinished( QNetworkReply *reply )
{
    QString response;
    if( !mCallMap.contains(reply) ) {
        return;
    }

    if( reply->error() != QNetworkReply::NoError ) {
        MaiaFault fault(-32300, reply->errorString());
        response = fault.toString();
    }
    else {
        response = QString::fromUtf8(reply->readAll());
    }

    // parseResponse deletes the MaiaObject
    mCallMap[reply]->slParseResponse(response, reply);
    reply->deleteLater();
    mCallMap.remove(reply);

} // void slReplyFinished( QNetworkReply *reply )

void MaiaXmlRpcClient::init()
{
    mRequest.setRawHeader("User-Agent", "libmaia/0.2");
    mRequest.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");

    connect(&mNam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slReplyFinished(QNetworkReply*)));

    connect(&mNam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
            this, SIGNAL(sgSslErrors(QNetworkReply*,QList<QSslError>)));

} // void init()
