/*
 * libMaia - maiaXmlRpcServer.h
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

#ifndef MAIAXMLRPCSERVER_H
#define MAIAXMLRPCSERVER_H

// CORE includes
#include <QHash>
#include <QList>
#include <QObject>

// NETWORK includes
#include <QHostAddress>
#include <QTcpServer>

// maia includes
#include "maia_global.h"

class MAIASHARED_EXPORT MaiaXmlRpcServer : public QObject
{
    Q_OBJECT

public:
    explicit MaiaXmlRpcServer( QObject *parent = 0 );

    bool listen( const QHostAddress &address = QHostAddress::Any, quint16 port = 0 );

    QString serverError() const;

    void addMethod( const QString &method, QObject *responseObject, const char *responseSlot );
    void removeMethod( const QString &method );

    void setAllowedAddresses( const QList<QHostAddress> &addressList );
    QList<QHostAddress> allowedAddresses() const;

    QHostAddress serverAddress() const;
    quint16 serverPort() const;

public slots:
    void slGetMethod( const QString &method, QObject **responseObject, const char **responseSlot );

private slots:
    void slNewConnection();

private:
    QTcpServer mServer;
    QHash<QString, QObject *> mObjectMap;
    QHash<QString, const char *> mSlotMap;
    QList<QHostAddress> mAllowedAddresses;

    friend class maiaXmlRpcServerConnection;

};

#endif
