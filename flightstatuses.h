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

#ifndef FLIGHTSTATUSES_H
#define FLIGHTSTATUSES_H

#include "xmldatasource.h"
#include <QHash>

class FlightStatuses : public XmlDataSource
{
    Q_OBJECT
    Q_PROPERTY(StatusCode statusCode READ statusCode WRITE setStatusCode NOTIFY statusCodeChanged)
    Q_PROPERTY(QString statusCodeString READ statusCodeString WRITE setStatusCodeString NOTIFY statusCodeChanged)
public:

    enum StatusCode {
        None,
        NewInfo,  // "N"
        NewTime,  // "E"
        Departed, // "D"
        Arrived,  // "A"
        Canceled  // "C"
    };

    FlightStatuses(QObject *parent = 0);
    ~FlightStatuses();

    StatusCode statusCode() const;
    void setStatusCode(StatusCode code);

    QString statusCodeString() const;
    void setStatusCodeString(const QString &code);

Q_SIGNALS:
    void statusCodeChanged();

public Q_SLOTS:
    QString statusTextEnglish(const QString &code) const;
    QString statusTextNorwegian(const QString &code) const;

protected Q_SLOTS:
    void createMapping(Node *root);

protected:
    static QString toString(StatusCode code);
    static StatusCode fromString(const QString &code);

private:
    StatusCode m_statusCode;
    QHash<QString, QString> m_mappingEnglish;
    QHash<QString, QString> m_mappingNorwegian;
};

#endif
