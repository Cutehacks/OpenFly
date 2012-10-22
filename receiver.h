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

#ifndef RECEIVER_H
#define RECEIVER_H

#include <QtCore>
#include <QtNetwork>
#include <QtGui>
#include "node.h"

class Receiver;

class RequestQueue : public QObject
{
    Q_OBJECT
public:
    RequestQueue(QObject *parent = 0);
    ~RequestQueue();

public Q_SLOTS:
    void enableCaching(bool enable);
    void setCacheSize(int size);
    void setCacheMechanism(QNetworkRequest::CacheLoadControl mechanism);
    void setCacheExpiration(int seconds);
    void sendRequest(const QUrl &url, Receiver *receiver, QNetworkAccessManager::Operation operation, const QByteArray &data);
    void setCookies(const QList<QNetworkCookie> &cookies);
    void setUserAgent(const QByteArray &userAgent);
    void setRawHeader(const QByteArray &headerName, const QByteArray &headerValue);
    void abort(Receiver *receiver);

protected Q_SLOTS:
    void replyFinished(QNetworkReply *reply);
    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

Q_SIGNALS:
    void error(Receiver *receiver, const QString &errorString);
    void replyFinished(Receiver *receiver, const QByteArray &reply, const QString &errorString);
    void authenticationRequired(Receiver *receiver, const QByteArray &reply, QAuthenticator *authenticator);

protected:
    QNetworkAccessManager *manager() const;
    void setManager(QNetworkAccessManager *manager);

private:
    QHash<QNetworkReply*, Receiver*> m_receivers;
    QNetworkAccessManager *m_manager;
    QList<QNetworkCookie> m_cookies;
    QHash <QByteArray, QByteArray> m_headerHash;
    int m_cacheMaxSize;
    QNetworkRequest::CacheLoadControl m_cacheMechanism;
    int m_cacheExpirationSeconds;
    friend class RequestQueueThread; // ### WARNING!!! Use with caution!
};

Q_DECLARE_METATYPE(QNetworkAccessManager::Operation)


class RequestQueueThread : public QThread
{
    Q_OBJECT
public:
    RequestQueueThread(QObject *parent=0);
    ~RequestQueueThread();

    static RequestQueueThread *instance();
    static QNetworkAccessManager *manager(); // ### WARNING!!! Use with caution!
    static void setManager(QNetworkAccessManager *manager);  // ### WARNING!!! Use with caution!

    void enableCaching(bool enable);
    void setCacheSize(int size);
    void setCacheMechanism(QNetworkRequest::CacheLoadControl mechanism);
    void setCacheExpiration(int seconds);
    void sendRequest(const QUrl &url, Receiver *receiver,
                     QNetworkAccessManager::Operation operation = QNetworkAccessManager::GetOperation,
                     const QByteArray &data = QByteArray());
    void setCookies(const QList<QNetworkCookie> &cookies);
    void setUserAgent(const QByteArray &userAgent);
    void setRawHeader(const QByteArray &headerName, const QByteArray &headerValue);
    void abort(Receiver *receiver);

protected Q_SLOTS:
    void error(Receiver *receiver, const QString &errorString);
    void replyFinished(Receiver *receiver, const QByteArray &reply, const QString &errorString);

Q_SIGNALS:
    void enableCachingSignal(bool enable);
    void setCacheSizeSignal(int size);
    void setCacheMechanismSignal(QNetworkRequest::CacheLoadControl mechanism);
    void setCacheExpirationSignal(int seconds);
    void sendRequestSignal(const QUrl &url, Receiver *receiver, QNetworkAccessManager::Operation operation, const QByteArray &data);
    void setCookiesSignal(const QList<QNetworkCookie> &cookies);
    void setUserAgentSignal(const QByteArray &userAgent);
    void setRawHeaderSignal(const QByteArray &headerName, const QByteArray &headerValue);
    void abortSignal(Receiver *receiver);

protected:
    void run();
    void connectToQueue(RequestQueue *queue);

private:
    static QMutex m_mutex;
    static QWaitCondition m_condition;
    static bool m_connected;
    static bool m_threaded;
    static RequestQueue *m_queue; // ### WARNING!!! Use with caution!
};

class Receiver : public QObject
{
    friend class RequestQueueThread;
    Q_OBJECT
public:
    Receiver(QObject *parent = 0);
    ~Receiver();

Q_SIGNALS:
    void finished();
    void error(const QString &error);

public Q_SLOTS:
    virtual void request(const QUrl &url);
    void abortRequest();

protected Q_SLOTS:
    virtual void replyFinished(const QByteArray &reply, const QString &errorString);
};

class NodeReceiver : public Receiver
{
    Q_OBJECT
public:
    NodeReceiver(QObject *parent = 0);
    ~NodeReceiver();

    void clear();
    Node *rootNode() const;
    void setRootNode(Node *node);

private:
    Node *m_root;
};

class XmlReceiver : public NodeReceiver
{
    Q_OBJECT
public:
    XmlReceiver(QObject *parent = 0);
    ~XmlReceiver();

protected:
    void replyFinished(const QByteArray &reply, const QString &errorString);
};

class JsonReceiver : public NodeReceiver
{
    Q_OBJECT
public:
    JsonReceiver(QObject *parent = 0);
    ~JsonReceiver();

protected:
    void replyFinished(const QByteArray &reply, const QString &errorString);
};

class PixmapReceiver : public Receiver
{
    Q_OBJECT
    Q_PROPERTY(QPixmap pixmap READ pixmap)
public:
    PixmapReceiver(QObject *parent = 0);
    QPixmap pixmap() const;
#if 0
    void request(const QUrl &url);
#endif
Q_SIGNALS:
    void finished(const QPixmap &pixmap);

protected:
    void replyFinished(const QByteArray &reply, const QString &errorString);

private:
    QPixmap m_pixmap;
#if 0
    QString m_path;
#endif
};

#endif
