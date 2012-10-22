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

#include "flightsmodel.h"

#include "settings.h"
#include "airportnames.h"
#include "flightstatuses.h"
#include "node.h"

FlightsModel::FlightsModel(Settings *settings,
                           AirportNames *airports,
                           FlightStatuses *statuses,
                           FlightTimes *times,
                           QObject *parent)
    : QAbstractListModel(parent),
      m_settings(settings),
      m_airports(airports),
      m_statuses(statuses),
      m_times(times)
{
    QHash<int,QByteArray> names;
    names.insert(0, "uniqueID");
    names.insert(1, "airline");
    names.insert(2, "flightID");
    names.insert(3, "domesticOrInternational");
    names.insert(4, "scheduleTime");
    names.insert(5, "arrivalOrDeparture");
    names.insert(6, "airportCode");
    names.insert(7, "airportName");
    names.insert(8, "viaAirportCodes");
    names.insert(9, "viaAirportNames");
    names.insert(10, "routeAirportNames");
    names.insert(11, "checkinArea");
    names.insert(12, "gate");
    names.insert(13, "statusCode");
    names.insert(14, "statusText");
    names.insert(15, "statusTime");
    names.insert(16, "beltNumber");
    names.insert(17, "roundtrip");
    names.insert(18, "scheduleText");
    setRoleNames(names);

    m_times->setAirportCode(m_settings->airportCode());
    QObject::connect(m_times, SIGNAL(dataUpdated(Node*)), this, SLOT(updateContents(Node*)));
    QObject::connect(m_times, SIGNAL(airportCodeChanged()), this, SIGNAL(airportCodeChanged()));
    QObject::connect(m_settings, SIGNAL(airportCodeChanged()), this, SLOT(reloadContents()));
}

FlightsModel::~FlightsModel()
{
}

int FlightsModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_flights.count();
}

bool FlightsModel::isLoading() const
{
    return m_flights.isEmpty();// && m_times && m_times->isWaiting();
}

QString FlightsModel::airportCode() const
{
    return m_times ? m_times->airportCode() : QString();
}

QVariant FlightsModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid()) {
        switch (role) {
        case 0: return m_flights.value(index.row()).m_uniqueID;
        case 1: return m_flights.value(index.row()).m_airline;
        case 2: return m_flights.value(index.row()).m_flightID;
        case 3: return m_flights.value(index.row()).m_domesticOrInternational;
        case 4: return m_flights.value(index.row()).m_scheduleTime;
        case 5: return m_flights.value(index.row()).m_arrivalOrDeparture;
        case 6: return m_flights.value(index.row()).m_airportCode;
        case 7: return m_flights.value(index.row()).m_airportName;
        case 8: return m_flights.value(index.row()).m_viaAirportCodes;
        case 9: return m_flights.value(index.row()).m_viaAirportNames;
        case 10: return m_flights.value(index.row()).m_routeAirportNames;
        case 11: return m_flights.value(index.row()).m_checkinArea;
        case 12: return m_flights.value(index.row()).m_gate;
        case 13: return m_flights.value(index.row()).m_statusCode;
        case 14: return m_flights.value(index.row()).m_statusText;
        case 15: return m_flights.value(index.row()).m_statusTime;
        case 16: return m_flights.value(index.row()).m_beltNumber;
        case 17: return m_flights.value(index.row()).m_roundtrip;
        case 18: return m_flights.value(index.row()).m_scheduleText;
        default: break;
        }
    }
    return QVariant();
}

void FlightsModel::reloadContents()
{
    if (m_times) {
        m_times->setAirportCode(m_settings->airportCode());
        m_times->start();
    }
    beginResetModel();
    m_flights.clear();
    endResetModel();
    emit loadingChanged();
}

void FlightsModel::updateContents(Node *root)
{
    beginResetModel();
    m_flights.clear();

    QStack<Node*> pending;
    pending.push(root);
    while (!pending.isEmpty()) {
        Node *node = pending.pop();
        for (int i = 0; i < node->childCount(); ++i) {
            Node *child = node->child(i);
            if (child->type() == "flight") {
                appendFlightData(child);
            } else {
                pending.push(child);
            }
        }
    }

    endResetModel();
    emit loadingChanged();
}

void FlightsModel::appendFlightData(Node *flight)
{
    const QString airportCode = m_times->airportCode();
    FlightData data;
    if (flight && flight->type() == "flight") {
        data.m_uniqueID = flight->attribute("uniqueID").toLong();
        if (Node *airline = flight->firstChildOfType("airline"))
            data.m_airline = airline->text();
        if (Node *flight_id = flight->firstChildOfType("flight_id"))
            data.m_flightID = flight_id->text();
        if (Node *dom_int = flight->firstChildOfType("dom_int"))
            data.m_domesticOrInternational = dom_int->text(); // "D" = domestic, "I" = international
        if (Node *schedule_time = flight->firstChildOfType("schedule_time")) {
            QDateTime scheduleTime = QDateTime::fromString(schedule_time->text(), Qt::ISODate);
            scheduleTime.setTimeSpec(Qt::UTC);
            data.m_scheduleTime = scheduleTime.toLocalTime();
            data.m_scheduleText = data.m_scheduleTime.toString("hhmm");
        }
        if (Node *arr_dep = flight->firstChildOfType("arr_dep"))
            data.m_arrivalOrDeparture = arr_dep->text(); // "D" = departure, "A" = arrival
        if (Node *airport = flight->firstChildOfType("airport")) {
            data.m_airportCode = airport->text();
            data.m_airportName = m_airports ? m_airports->airportName(data.m_airportCode) : QString();
            data.m_roundtrip = (data.m_airportCode == airportCode); // if destination and origin are the same
        }
        if (Node *via_airport = flight->firstChildOfType("via_airport")) {
            data.m_viaAirportCodes = via_airport->text().split(',');
            foreach(QString code, data.m_viaAirportCodes)
                data.m_viaAirportNames.append(m_airports->airportName(code));
            if (!data.m_roundtrip && data.m_arrivalOrDeparture == "D")
                data.m_viaAirportNames.append(data.m_airportName);
            data.m_routeAirportNames = data.m_viaAirportNames.join(", ");
        }
        if (Node *check_in = flight->firstChildOfType("check_in"))
            data.m_checkinArea = check_in->text(); // "A" "B" "C" "D" checking area
        if (Node *gate = flight->firstChildOfType("gate"))
            data.m_gate = gate->text();
        if (Node *status = flight->firstChildOfType("status")) {
            data.m_statusCode = status->attribute("code");
            data.m_statusText = m_statuses ? m_statuses->statusTextNorwegian(data.m_statusCode) : QString(); // ### support norwegian
            QDateTime statusTime = QDateTime::fromString(status->attribute("time"), Qt::ISODate);
            statusTime.setTimeSpec(Qt::UTC);
            data.m_statusText += statusTime.toLocalTime().toString(" hh:mm");
        }
        if (Node *belt_number = flight->firstChildOfType("belt_number"))
            data.m_beltNumber = belt_number->text();
    }
    m_flights.append(data);
}
