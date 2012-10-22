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

#include "receiver.h"
#include <QNetworkRequest>
#include <QMutexLocker>

Q_GLOBAL_STATIC(RequestQueueThread, _requestQueueThread)

RequestQueue::RequestQueue(QObject *parent)
    : QObject(parent),
      m_manager(0),
      m_cacheMaxSize(512*1024),
      m_cacheMechanism(QNetworkRequest::PreferNetwork),
      m_cacheExpirationSeconds(0)
{
    qRegisterMetaType<QNetworkAccessManager::Operation>("Operation");
    setManager(new QNetworkAccessManager(this));
}

RequestQueue::~RequestQueue()
{
}

QNetworkAccessManager *RequestQueue::manager() const
{
    return m_manager;
}

void RequestQueue::setManager(QNetworkAccessManager *manager)
{
    delete m_manager;
    m_manager = manager;
    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

void RequestQueue::enableCaching(bool enable)
{
    if (enable) {
        if (m_manager->cache() == 0) {
            QNetworkDiskCache *cache = new QNetworkDiskCache(this);
            cache->setCacheDirectory(".");
            cache->setMaximumCacheSize(m_cacheMaxSize);
            m_manager->setCache(cache);
        }
    } else if (m_manager->cache() != 0) {
        m_manager->setCache(0);
    }
}

void RequestQueue::setCacheSize(int size)
{
    m_cacheMaxSize = size;
    if (QNetworkDiskCache *cache = qobject_cast<QNetworkDiskCache *>(m_manager->cache()))
        cache->setMaximumCacheSize(m_cacheMaxSize);
}

void RequestQueue::setCacheMechanism(QNetworkRequest::CacheLoadControl mechanism)
{
    m_cacheMechanism = mechanism;
}

/*!
  Overrides expiration of cached network requests to QDateTime::currentDateTime() + seconds.
  Existing cache is not affected, only new requests will be overriden.
  A value lower or equeal to 0 leaves network traffic untouched and keeps original expiration date as specified from the server.

  Default value is 0
*/
void RequestQueue::setCacheExpiration(int seconds)
{
    m_cacheExpirationSeconds = seconds;
}

void RequestQueue::sendRequest(const QUrl &url, Receiver *receiver, QNetworkAccessManager::Operation operation, const QByteArray &data)
{
    QNetworkRequest request(url);
    if (!m_cookies.isEmpty())
        request.setHeader(QNetworkRequest::CookieHeader, qVariantFromValue(m_cookies));
    QHashIterator<QByteArray, QByteArray> it(m_headerHash);
    while (it.hasNext()) {
        it.next();
        request.setRawHeader(it.key(), it.value());
    }
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, m_cacheMechanism);
    QNetworkReply *reply = 0;
    switch (operation) {
    case QNetworkAccessManager::HeadOperation:
        reply = m_manager->head(request);
        break;
    case QNetworkAccessManager::GetOperation:
        reply = m_manager->get(request);
        break;
    case QNetworkAccessManager::PutOperation:
        reply = m_manager->put(request, data);
        break;
    case QNetworkAccessManager::DeleteOperation:
        reply = m_manager->deleteResource(request);
        break;
    default:
        break;
    }
    if (reply && reply->error() != QNetworkReply::NoError) {
        emit error(receiver, reply->errorString());
        reply->deleteLater();
    } else if (reply) {
        m_receivers.insert(reply, receiver);
    }
}

void RequestQueue::setCookies(const QList<QNetworkCookie> &cookies)
{
    m_cookies = cookies;
}

void RequestQueue::setUserAgent(const QByteArray &userAgent)
{
    setRawHeader("User-Agent", userAgent);
}

void RequestQueue::setRawHeader(const QByteArray &headerName, const QByteArray &headerValue)
{
    m_headerHash.insert(headerName, headerValue);
}

void RequestQueue::abort(Receiver *receiver)
{
    if (QNetworkReply *reply = m_receivers.key(receiver, 0))
        reply->abort();
}

void RequestQueue::replyFinished(QNetworkReply *reply)
{
    if (Receiver *receiver = m_receivers.value(reply, 0))
        emit replyFinished(receiver, reply->readAll(), reply->errorString());

    if (m_cacheExpirationSeconds > 0 && m_manager->cache() && !reply->attribute(QNetworkRequest::SourceIsFromCacheAttribute).toBool()) {
        QNetworkCacheMetaData metaData = m_manager->cache()->metaData(reply->url());
        QDateTime oldExpire = metaData.expirationDate();
        metaData.setExpirationDate(QDateTime::currentDateTime().addSecs(m_cacheExpirationSeconds));
        m_manager->cache()->updateMetaData(metaData);
    }
    reply->deleteLater();
}

void RequestQueue::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    if (Receiver *receiver = m_receivers.value(reply, 0))
        emit authenticationRequired(receiver, reply->readAll(), authenticator);
}

// RequestQueueThread

QMutex RequestQueueThread::m_mutex;
QWaitCondition RequestQueueThread::m_condition;
bool RequestQueueThread::m_connected = false;
bool RequestQueueThread::m_threaded = true;
RequestQueue *RequestQueueThread::m_queue = 0;

RequestQueueThread::RequestQueueThread(QObject *parent)
    : QThread(parent)
{
}

RequestQueueThread::~RequestQueueThread()
{
    quit();
    wait();
}

RequestQueueThread *RequestQueueThread::instance()
{
    RequestQueueThread *queueThread = _requestQueueThread();
    if (m_threaded) {
        if (!queueThread->isRunning())
            queueThread->start();
        QMutexLocker locker(&m_mutex);
        while (!m_connected)
            m_condition.wait(&m_mutex);
    } else { // unthreaded
        if (!m_queue) {
            m_queue = new RequestQueue(queueThread);
            queueThread->connectToQueue(m_queue);
            m_connected = true;
        }
    }
    return queueThread;
}

QNetworkAccessManager *RequestQueueThread::manager()
{
    m_threaded = false;
    return instance()->m_queue->manager();
}

void RequestQueueThread::setManager(QNetworkAccessManager *manager)
{
    m_threaded = false;
    instance()->m_queue->setManager(manager);
}

void RequestQueueThread::enableCaching(bool enable)
{
    emit enableCachingSignal(enable);
}

void RequestQueueThread::setCacheSize(int size)
{
    emit setCacheSizeSignal(size);
}

void RequestQueueThread::setCacheMechanism(QNetworkRequest::CacheLoadControl mechanism)
{
    emit setCacheMechanismSignal(mechanism);
}

void RequestQueueThread::setCacheExpiration(int seconds)
{
    emit setCacheExpirationSignal(seconds);
}

void RequestQueueThread::sendRequest(const QUrl &url, Receiver *receiver, QNetworkAccessManager::Operation operation, const QByteArray &data)
{
    emit sendRequestSignal(url, receiver, operation, data);
}

void RequestQueueThread::setCookies(const QList<QNetworkCookie> &cookies)
{
    emit setCookiesSignal(cookies);
}

void RequestQueueThread::setUserAgent(const QByteArray &userAgent)
{
    emit setUserAgentSignal(userAgent);
}

void RequestQueueThread::setRawHeader(const QByteArray &headerName, const QByteArray &headerValue)
{
    emit setRawHeaderSignal(headerName, headerValue);
}

void RequestQueueThread::abort(Receiver *receiver)
{
    emit abortSignal(receiver);
}

void RequestQueueThread::error(Receiver *receiver, const QString &errorString)
{
    if (receiver)
        receiver->error(errorString);
}

void RequestQueueThread::replyFinished(Receiver *receiver, const QByteArray &reply, const QString &errorString)
{
    if (receiver)
        receiver->replyFinished(reply, errorString);
}

void RequestQueueThread::run()
{
    RequestQueue queue;
    connectToQueue(&queue);

    // start eventloop for this thread

    m_mutex.lock();
    m_connected = true;
    m_condition.wakeAll();
    m_mutex.unlock();

    exec();
}

void RequestQueueThread::connectToQueue(RequestQueue *queue)
{
    // connections from Thread to Queue
    connect(this, SIGNAL(enableCachingSignal(bool)), queue, SLOT(enableCaching(bool)));
    connect(this, SIGNAL(setCacheSizeSignal(int)), queue, SLOT(setCacheSize(int)));
    qRegisterMetaType<QNetworkRequest::CacheLoadControl>("QNetworkRequest::CacheLoadControl");
    connect(this, SIGNAL(setCacheMechanismSignal(QNetworkRequest::CacheLoadControl)), queue, SLOT(setCacheMechanism(QNetworkRequest::CacheLoadControl)));
    connect(this, SIGNAL(setCacheExpirationSignal(int)), queue, SLOT(setCacheExpiration(int)));
    connect(this, SIGNAL(sendRequestSignal(QUrl,Receiver*,QNetworkAccessManager::Operation,QByteArray)),
            queue, SLOT(sendRequest(QUrl,Receiver*,QNetworkAccessManager::Operation,QByteArray)));
    connect(this, SIGNAL(setCookiesSignal(QList<QNetworkCookie>)), queue, SLOT(setCookies(QList<QNetworkCookie>)));
    connect(this, SIGNAL(setUserAgentSignal(QByteArray)), queue, SLOT(setUserAgent(QByteArray)));
    connect(this, SIGNAL(setRawHeaderSignal(QByteArray,QByteArray)), queue, SLOT(setRawHeader(QByteArray,QByteArray)));

    // connections from Queue to Thread
    connect(queue, SIGNAL(error(Receiver*,QString)), this, SLOT(error(Receiver*,QString)));
    connect(queue, SIGNAL(replyFinished(Receiver*,QByteArray,QString)), this, SLOT(replyFinished(Receiver*,QByteArray,QString)));
}

// Receiver

Receiver::Receiver(QObject *parent)
    : QObject(parent)
{
}

Receiver::~Receiver()
{
}

void Receiver::request(const QUrl &url)
{
    RequestQueueThread::instance()->sendRequest(url, this);
}

void Receiver::abortRequest()
{
    RequestQueueThread::instance()->abort(this);
}

void Receiver::replyFinished(const QByteArray &reply, const QString &errorString)
{
    Q_UNUSED(reply);
    Q_UNUSED(errorString);
    emit finished();
}

NodeReceiver::NodeReceiver(QObject *parent)
    : Receiver(parent), m_root(0)
{
}

NodeReceiver::~NodeReceiver()
{
    clear();
}

void NodeReceiver::clear()
{
    delete m_root;
    m_root = 0;
}

Node *NodeReceiver::rootNode() const
{
    return m_root;
}

void NodeReceiver::setRootNode(Node *node)
{
    m_root = node;
}

XmlReceiver::XmlReceiver(QObject *parent)
    : NodeReceiver(parent)
{
}

XmlReceiver::~XmlReceiver()
{
}

void XmlReceiver::replyFinished(const QByteArray &reply, const QString &errorString)
{
    clear();
    setRootNode(new Node("root"));
    Node *current = rootNode();
    if (reply.isEmpty())
        emit error(errorString);
    QXmlStreamReader xml(reply);
    while (!xml.atEnd()) {
        switch(xml.readNext()) {
            case QXmlStreamReader::NoToken:
                break;
            case QXmlStreamReader::Invalid:
                break;
            case QXmlStreamReader::StartDocument:
                break;
            case QXmlStreamReader::EndDocument:
                break;
            case QXmlStreamReader::StartElement: {
                const QString name = xml.name().toString();
                current = new Node(name, current);
                foreach (QXmlStreamAttribute attribute, xml.attributes())
                    current->setAttribute(attribute.name().toString(), attribute.value().toString());
                break; }
            case QXmlStreamReader::EndElement:
                current = current->parent();
                break;
            case QXmlStreamReader::Characters:
                if (current && !xml.isWhitespace())
                    current->setText(xml.text().toString());
                break;
            case QXmlStreamReader::Comment:
                break;
            case QXmlStreamReader::DTD:
                break;
            case QXmlStreamReader::EntityReference:
                break;
            case QXmlStreamReader::ProcessingInstruction:
                break;
            default:
                break;
        }
    }
    if (xml.hasError())
        qDebug() << xml.errorString();
    emit finished();
}

JsonReceiver::JsonReceiver(QObject *parent)
    : NodeReceiver(parent)
{
}

JsonReceiver::~JsonReceiver()
{
}

void JsonReceiver::replyFinished(const QByteArray &reply, const QString &errorString)
{
    clear();
    setRootNode(new Node("root"));
    Node *current = rootNode();
    if (reply.isEmpty())
        emit error(errorString);
    // FIXME: a very simple, naive json parser - will break
    QByteArray element;
    QByteArray property;
    for (int i = 0; i < reply.size(); ++i) {
        switch (reply.at(i)) {
        case '{': // start object
            if (!property.isEmpty()) { // object is property value
                current = new Node("element", current);
                current->setText(QString::fromUtf8(property.data(), property.size()));
            }
            current = new Node("object", current);
            break;
        case '}': // end object
            do {
                current = current ? current->parent() : 0;
            } while (current && current->type() != "object");
            if (!element.isEmpty()) {
                Node *node = new Node("element", current);
                if (!property.isEmpty())
                    node->setAttribute(property, element);
                else
                    node->setText(QString::fromUtf8(element.data(), element.size()));
                property.clear();
                element.clear();
            }
            break;
        case ':': // text was propery name
            property = element;
            element.clear();
            break;
        case '[': // start list
            if (!property.isEmpty()) { // list is property value
                current = new Node("element", current);
                current->setText(QString::fromUtf8(property.data(), property.size()));
            }
            current = new Node("list", current);
            break;
        case ']': // end list
            if (!element.isEmpty()) { // add last element
                (new Node("element", current))->setText(QString::fromUtf8(element.data(), element.size()));
                element.clear();
            }
            current = current ? current->parent() : 0;
            break;
        case ',': { // end element
            if (!element.isEmpty()) {
                Node *node = new Node("element", current);
                if (!property.isEmpty())
                    node->setAttribute(property, element);
                else
                    node->setText(QString::fromUtf8(element.data(), element.size()));
                property.clear();
                element.clear();
            }
            break; }
        case '"':
                while (reply.at(++i) != '"')
                    element.append(reply.at(i));
                break;
            break; // ignore
        default:
            element.append(reply.at(i));
            break;
        }
    }
    emit finished();
}

PixmapReceiver::PixmapReceiver(QObject *parent)
    : Receiver(parent)
{
}

QPixmap PixmapReceiver::pixmap() const
{
    return m_pixmap;
}
#if 0
void PixmapReceiver::request(const QUrl &url)
{
    const QString path = url.path();
    m_path = QDir::currentPath() + path.mid(path.lastIndexOf('/'));
    if (!m_path.isEmpty() && QFile::exists(m_path))
        m_pixmap.load(m_path);
    else
        Receiver::request(url)
}
#endif
void PixmapReceiver::replyFinished(const QByteArray &reply, const QString &errorString)
{
    if (reply.isEmpty())
        emit error(errorString);
    m_pixmap.loadFromData(reply);
    emit finished(m_pixmap);
#if 0
    // cache file
    if (!m_path.isEmpty() && !QFile::exists(m_path))
        m_pixmap.save(m_path, "PNG");
#endif
}
