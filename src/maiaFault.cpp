/*
 * libMaia - maiaFault.cpp
 * Copyright (c) 2007 Sebastian Wiedenroth <wiedi@frubar.net>
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

// maia includes
#include "maiaObject.h"

#include "maiaFault.h"

MaiaFault::MaiaFault( int faultCode, const QString &faultString, QObject *parent )
    : QObject(parent)
{
    mFault["faultCode"] = faultCode;
    mFault["faultString"] = faultString;

} // ctor

MaiaFault::MaiaFault( const MaiaFault &other )
    : QObject(other.parent())
{
    mFault = other.mFault;

} // copy ctor

QString MaiaFault::toString() const
{
    QDomDocument doc;
    QDomProcessingInstruction header = doc.createProcessingInstruction("xml", QString("version=\"1.0\" encoding=\"UTF-8\""));
    doc.appendChild(header);

    QDomElement methodResponse = doc.createElement("methodResponse");
    doc.appendChild(methodResponse);

    QDomElement faultelement = doc.createElement("fault");
    methodResponse.appendChild(faultelement);
    faultelement.appendChild(MaiaObject::toXml(mFault));
    return doc.toString();

} // QString toString() const
