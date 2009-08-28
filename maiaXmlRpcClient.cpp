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

#include "maiaXmlRpcClient.h"
#include "maiaFault.h"

MaiaXmlRpcClient::MaiaXmlRpcClient(QObject* parent) : QObject(parent) {
	http = new QHttp(this);
}

MaiaXmlRpcClient::MaiaXmlRpcClient(QUrl url, QObject* parent) : QObject(parent) {
	http = new QHttp(this);
	setUrl(url);
}

void MaiaXmlRpcClient::setUrl(QUrl url) {
	if(!url.isValid())
		return;
		
	m_url = url;
	http->setHost(m_url.host(), m_url.port() != -1 ? m_url.port() : 80);
	if (!m_url.userName().isEmpty())
		http->setUser(m_url.userName(), m_url.password());
}

void MaiaXmlRpcClient::call(QString method, QList<QVariant> args,
							QObject* responseObject, const char* responseSlot,
							QObject* faultObject, const char* faultSlot) {
	int callid = 0;
	MaiaObject* call = new MaiaObject(this);
	connect(call, SIGNAL(aresponse(QVariant &)), responseObject, responseSlot);
	connect(call, SIGNAL(fault(int, const QString &)), faultObject, faultSlot);

	connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestDone(int, bool)));
        connect(http, SIGNAL(responseHeaderReceived(QHttpResponseHeader)), this, SLOT(responseHeaderReceived(QHttpResponseHeader)));
	
	QHttpRequestHeader header("POST", m_url.path());
	header.setValue("Host", m_url.host()); 
	header.setValue("User-Agent", "libmaia 0.1");
	header.setValue("Content-type", "text/xml");
        if (cookie.length()>0) header.setValue("Cookie", cookie);
	callid = http->request(header, call->prepareCall(method, args).toUtf8());
	callmap[callid] = call;
}

void MaiaXmlRpcClient::httpRequestDone(int id, bool error) {
	QString response;
	if(!callmap.contains(id))
		return;
	if(error) {
		MaiaFault fault(-32300, http->errorString());
		response = fault.toString();
	} else {
		response = QString::fromUtf8(http->readAll());
	}
	callmap[id]->parseResponse(response);
	callmap.remove(id);
}

void MaiaXmlRpcClient::responseHeaderReceived(QHttpResponseHeader header) {
    if (header.keys().contains("Set-Cookie")) {
        cookie = header.value("Set-Cookie");
    }
}
