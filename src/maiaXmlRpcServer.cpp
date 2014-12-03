/*
 * libMaia - maiaXmlRpcServer.cpp
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
#include <QTcpSocket>

// maia includes
#include "maiaFault.h"
#include "maiaObject.h"
#include "maiaXmlRpcServerConnection.h"

#include "maiaXmlRpcServer.h"

MaiaXmlRpcServer::MaiaXmlRpcServer( QObject *parent )
    : QObject(parent)
{
    connect(&mServer, SIGNAL(newConnection()), this, SLOT(slNewConnection()));
    mServer.listen(QHostAddress::Any, 8080);

} // ctor

MaiaXmlRpcServer::MaiaXmlRpcServer( const QHostAddress &address, quint16 port, QObject *parent )
    : QObject(parent)
{
    connect(&mServer, SIGNAL(newConnection()), this, SLOT(slNewConnection()));
    mServer.listen(address, port);

} // ctor

void MaiaXmlRpcServer::addMethod( const QString &method, QObject *responseObject, const char *responseSlot )
{
    mObjectMap[method] = responseObject;
    mSlotMap[method] = responseSlot;

} // void addMethod( const QString &method, QObject *responseObject, const char *responseSlot )

void MaiaXmlRpcServer::removeMethod( const QString &method )
{
    mObjectMap.remove(method);
    mSlotMap.remove(method);

} // void removeMethod( const QString &method )

void MaiaXmlRpcServer::setAllowedAddresses( const QList<QHostAddress> &addressList )
{
    mAllowedAddresses = addressList;

} // void setAllowedAddresses( const QList<QHostAddress> &addressList )

QList<QHostAddress> MaiaXmlRpcServer::allowedAddresses() const
{
    return mAllowedAddresses;

} // QList<QHostAddress> allowedAddresses() const

QHostAddress MaiaXmlRpcServer::getServerAddress() const
{
    return mServer.serverAddress();

} // QHostAddress getServerAddress() const

void MaiaXmlRpcServer::slGetMethod( const QString &method, QObject **responseObject, const char **responseSlot )
{
    if( !mObjectMap.contains(method) ) {
        *responseObject = NULL;
        *responseSlot = NULL;
        return;
    }
    *responseObject = mObjectMap[method];
    *responseSlot = mSlotMap[method];

} // void slGetMethod( const QString &method, QObject **responseObject, const char **responseSlot )

void MaiaXmlRpcServer::slNewConnection()
{
    QTcpSocket *connection = mServer.nextPendingConnection();
    if( mAllowedAddresses.isEmpty() || mAllowedAddresses.contains(connection->peerAddress()) ) {
        MaiaXmlRpcServerConnection *client = new MaiaXmlRpcServerConnection(connection, this);
        connect(client, SIGNAL(sgGetMethod(QString,QObject**,const char**)), this, SLOT(slGetMethod(QString,QObject**,const char**)));
    }
    else {
        qWarning() << "Rejected connection attempt from" << connection->peerAddress().toString();
        connection->disconnectFromHost();
    }

} // void slNewConnection()

