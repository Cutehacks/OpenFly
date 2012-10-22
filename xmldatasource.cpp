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

#include "xmldatasource.h"
#include "receiver.h"
#include "node.h"

XmlDataSource::XmlDataSource(QObject *parent)
    : QObject(parent),
      m_receiver(new XmlReceiver(this)),
      m_refreshRate(-1),
      m_waiting(false)
{
    QObject::connect(m_receiver, SIGNAL(finished()), this, SLOT(dataReceived()));
}

XmlDataSource::~XmlDataSource()
{
}

int XmlDataSource::refreshRate() const
{
    return m_refreshRate;
}

void XmlDataSource::setRefreshRate(int rate)
{
    m_refreshRate = rate;
}

QUrl XmlDataSource::url() const
{
    return m_url;
}

void XmlDataSource::setUrl(const QUrl &url)
{
    m_url = url;
}

bool XmlDataSource::isWaiting() const
{
    return m_waiting;
}

void XmlDataSource::insertQuery(const QString &name, const QString &value)
{
    if (value.isNull())
        m_queryItems.remove(name);
    else
        m_queryItems.insert(name, value);
}

void XmlDataSource::removeQuery(const QString &name)
{
    m_queryItems.remove(name);
}

void XmlDataSource::start()
{
    // FIXME: check if there are cached data and if they are not too old
    executeQuery();
    if (m_refreshRate != -1 && !m_timer.isActive())
        m_timer.start(m_refreshRate, this);
}

void XmlDataSource::stop()
{
    m_timer.stop();
    m_waiting = false;
    m_receiver->abortRequest();
}

void XmlDataSource::dataReceived()
{
    m_timestamp = QDateTime::currentDateTime();
    m_waiting = false;
    emit dataUpdated(root());
}

void XmlDataSource::executeQuery()
{
    m_waiting = true;
    QList<QPair<QString, QString> > query;
    const QList<QString> keys = m_queryItems.keys();
    foreach (QString key, keys)
        query.append(QPair<QString,QString>(key, m_queryItems.value(key)));
    m_url.setQueryItems(query);
    m_receiver->request(m_url);
}

void XmlDataSource::timerEvent(QTimerEvent *event)
{
    if (m_timer.timerId() == event->timerId())
        executeQuery();
}

Node *XmlDataSource::root() const
{
    return m_receiver->rootNode();
}
