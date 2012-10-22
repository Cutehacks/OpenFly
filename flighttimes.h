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

#ifndef FLIGHTTIMES_H
#define FLIGHTTIMES_H

#include "xmldatasource.h"

class FlightTimes : public XmlDataSource
{
    Q_OBJECT
    Q_PROPERTY(int timeFrom READ timeFrom WRITE setTimeFrom NOTIFY timeFromChanged)
    Q_PROPERTY(int timeTo READ timeTo WRITE setTimeTo NOTIFY timeToChanged)
    Q_PROPERTY(QString airportCode READ airportCode WRITE setAirportCode NOTIFY airportCodeChanged)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QString directionString READ directionString WRITE setDirectionString NOTIFY directionChanged)
    Q_PROPERTY(QDateTime lastUpdate READ lastUpdate WRITE setLastUpdate NOTIFY lastUpdateChanged)

public:

    enum Direction { All, Departures, Arrivals };

    FlightTimes(Direction direction = All, QObject *parent = 0);
    ~FlightTimes();

    int timeFrom() const;
    void setTimeFrom(int time);

    int timeTo() const;
    void setTimeTo(int time);

    QString airportCode() const;
    void setAirportCode(const QString &code);

    Direction direction() const;
    void setDirection(Direction direction);

    QString directionString() const;
    void setDirectionString(const QString &direction);

    QDateTime lastUpdate() const;
    void setLastUpdate(const QDateTime &time);

Q_SIGNALS:
    void timeFromChanged();
    void timeToChanged();
    void airportCodeChanged();
    void directionChanged();
    void lastUpdateChanged();

protected:
    static QString toString(Direction direction);
    static Direction fromString(const QString &direction);

private:
    int m_timeFrom;
    int m_timeTo;
    QString m_airportCode;
    Direction m_direction;
    QDateTime m_lastUpdate;
};

#endif
