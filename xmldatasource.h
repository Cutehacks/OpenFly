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

#ifndef XMLDATASOURCE_H
#define XMLDATASOURCE_H

#include <QHash>
#include <QObject>
#include <QBasicTimer>
#include <QDateTime>
#include <QUrl>

class XmlReceiver;
class Node;

class XmlDataSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int refershRate READ refreshRate WRITE setRefreshRate)
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(bool waiting READ isWaiting)
public:
    XmlDataSource(QObject *parent = 0);
    ~XmlDataSource();

    int refreshRate() const;
    void setRefreshRate(int rate);

    QUrl url() const;
    void setUrl(const QUrl &url);

    bool isWaiting() const;

    void insertQuery(const QString &name, const QString &value);
    void removeQuery(const QString &name);

Q_SIGNALS:
    void dataUpdated(Node *root);

public Q_SLOTS:
    void start();
    void stop();

protected Q_SLOTS:
    void dataReceived();
    void executeQuery();

protected:
    void timerEvent(QTimerEvent *event);

    Node *root() const;

private:
    XmlReceiver *m_receiver;
    QBasicTimer m_timer;
    int m_refreshRate;
    QUrl m_url;
    bool m_waiting;
    QDateTime m_timestamp;
    QHash<QString, QString> m_queryItems;
};

#endif
