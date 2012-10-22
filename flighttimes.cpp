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

#include "flighttimes.h"
#include "node.h"

#include <QStack>

FlightTimes::FlightTimes(Direction direction, QObject *parent)
    : XmlDataSource(parent),
    m_timeFrom(1), // default
    m_timeTo(7), // default
    m_direction(direction)
{
    // "http://flydata.avinor.no/XmlFeed.asp?TimeFrom=1&TimeTo=7&airport=OSL&direction=D&lastUpdate=2009-03-10T15:03:00"
    setUrl(QUrl("http://flydata.avinor.no/XmlFeed.asp"));
    setRefreshRate(180000); //msecs == 3m
    setTimeFrom(1);
    setTimeTo(7);
    setDirection(direction);
}

FlightTimes::~FlightTimes()
{
}

int FlightTimes::timeFrom() const
{
    return m_timeFrom;
}

/*!
  Number of hours back in time the flight times start.
  Non-obligatory.
*/
void FlightTimes::setTimeFrom(int time)
{
    m_timeFrom = time;
    insertQuery("TimeFrom", QString::number(time));
    emit timeFromChanged();
}

/*!
  Number of hours forwards in time the flight times end.
  Non-obligatory.
*/
int FlightTimes::timeTo() const
{
    return m_timeTo;
}

void FlightTimes::setTimeTo(int time)
{
    m_timeTo = time;
    insertQuery("TimeTo", QString::number(time));
    emit timeToChanged();
}

QString FlightTimes::airportCode() const
{
    return m_airportCode;
}

void FlightTimes::setAirportCode(const QString &code)
{
    m_airportCode = code;
    insertQuery("airport", code);
    emit airportCodeChanged();
}

FlightTimes::Direction FlightTimes::direction() const
{
    return m_direction;
}

void FlightTimes::setDirection(Direction direction)
{
    m_direction = direction;
    insertQuery("direction", toString(direction));
    emit directionChanged();
}

QString FlightTimes::directionString() const
{
    return toString(direction());
}

void FlightTimes::setDirectionString(const QString &direction)
{
    setDirection(fromString(direction));
}

QDateTime FlightTimes::lastUpdate() const
{
    return m_lastUpdate;
}

void FlightTimes::setLastUpdate(const QDateTime &time)
{
    m_lastUpdate = time;
    insertQuery("lastUpdate", time.toString(Qt::ISODate));
    emit lastUpdateChanged();
}

QString FlightTimes::toString(Direction direction)
{
    switch (direction) {
    case Departures: return "D";
    case Arrivals: return "A";
    case All:
    default:
        break;
    }
    return QString();
}

FlightTimes::Direction FlightTimes::fromString(const QString &direction)
{
    if (direction == "D")
        return Departures;
    if (direction == "A")
        return Arrivals;
    return All;
}
