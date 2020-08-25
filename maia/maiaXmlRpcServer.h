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

#include <QtCore>
#include <QtXml>
#include <QtNetwork>

#include "maiaObject.h"
#include "maiaXmlRpcServerConnection.h"

class MaiaXmlRpcServer : public QObject {
	Q_OBJECT
	
	public:
		MaiaXmlRpcServer(QObject* parent);
		MaiaXmlRpcServer(const QHostAddress &address = QHostAddress::Any, quint16 port = 8080, QList<QHostAddress> *allowedAddresses = 0, QObject *parent = 0);
		MaiaXmlRpcServer(const QHostAddress &address, quint16 port, QObject* parent);
		MaiaXmlRpcServer(quint16 port, QObject* parent = 0);

		void addMethod(QString method, QObject *responseObject, const char* responseSlot);
		void removeMethod(QString method);

		const QList<QHostAddress> *allowedAddresses() const;
		void setAllowedAddresses(QList<QHostAddress> *allowedAddresses);
		void setAllowedAddresses(const QList<QHostAddress> &allowedAddresses);

		const QMap<QString, QString> &authorizedUsers() const;
		void setAuthorizedUsers(const QMap<QString, QString> &authorizedUsers);

		bool allowPersistentConnections() const;
		void setAllowPersistentConnections(bool allowPersistentConnections);

		bool isListening() const;
		QHostAddress getServerAddress() const;
		quint16 getServerPort() const;

		bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 8080);
		void close();

	public slots:
		void getMethod(QString method, QObject **responseObject, const char** responseSlot);
	
	private slots:
		void newConnection();
	
	private:
		QTcpServer server;
		QHash<QString, QObject*> objectMap;
		QHash<QString, const char*> slotMap;
		QList<QHostAddress> localAllowedAddresses;
		QList<QHostAddress> *m_allowedAddresses;
		QMap<QString, QString> m_authorizedUsers;
		bool m_allowPersistentConnections;
		
	friend class maiaXmlRpcServerConnection;
		
};

#endif
