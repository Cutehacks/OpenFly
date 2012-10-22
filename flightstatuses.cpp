/*
 *  Copyright (C) 2012 Cutehacks AS. All rights reserved.
 *  info@cutehacks.com
 *  http://cutehacks.com
 *
 *  This file is part of Fly.
 *
 *  Fly is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Fly is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Fly.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "flightstatuses.h"
#include "node.h"
#include <QStack>

FlightStatuses::FlightStatuses(QObject *parent)
    : XmlDataSource(parent)
{
    // use http://flydata.avinor.no/flightStatuses.asp?code=A for specific statuses
    setUrl(QUrl("http://flydata.avinor.no/flightStatuses.asp"));
    setRefreshRate(86400000); //msecs == 24h
    QObject::connect(this, SIGNAL(dataUpdated(Node*)), this, SLOT(createMapping(Node*)));
}

FlightStatuses::~FlightStatuses()
{
}

FlightStatuses::StatusCode FlightStatuses::statusCode() const
{
    return m_statusCode;
}

void FlightStatuses::setStatusCode(StatusCode code)
{
    m_statusCode = code;
    insertQuery("code", toString(code));
    emit statusCodeChanged();
}

QString FlightStatuses::statusCodeString() const
{
    return toString(statusCode());
}

void FlightStatuses::setStatusCodeString(const QString &code)
{
    setStatusCode(fromString(code));
}

QString FlightStatuses::statusTextEnglish(const QString &code) const
{
    return m_mappingEnglish.value(code, code);
}

QString FlightStatuses::statusTextNorwegian(const QString &code) const
{
    return m_mappingNorwegian.value(code, code);
}

void FlightStatuses::createMapping(Node *root)
{
    m_mappingEnglish.clear();
    m_mappingNorwegian.clear();
    QStack<Node*> pending;
    pending.push(root);
    while (!pending.isEmpty()) {
        Node *node = pending.pop();
        if (node->type() == "flightStatus") {
            const QString code = node->attribute("code");
            m_mappingEnglish.insert(code, node->attribute("statusTextEn"));
            m_mappingNorwegian.insert(code, node->attribute("statusTextNo"));
        }
        for (int i = 0; i < node->childCount(); ++i)
            pending.push(node->child(i));
    }
}

QString FlightStatuses::toString(StatusCode code)
{
    switch (code) {
    case NewInfo: return "N";
    case NewTime: return "E";
    case Departed: return "D";
    case Arrived: return "A";
    case Canceled: return "C";
    case None:
    default:
        break;
    }
    return QString();
}

FlightStatuses::StatusCode FlightStatuses::fromString(const QString &code)
{
    if (code == "N")
        return NewInfo;
    if (code == "E")
        return NewTime;
    if (code == "D")
        return Departed;
    if (code == "A")
        return Arrived;
    if (code == "C")
        return Canceled;
    return None;
}
