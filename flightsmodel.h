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

#ifndef FLIGHTSMODEL_H
#define FLIGHTSMODEL_H

#include <QAbstractListModel>
#include <QStringList>

#include "flighttimes.h"

class Node;
class AirportNames;
class FlightStatuses;
class Settings;

class FlightsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)
    Q_PROPERTY(QString airportCode READ airportCode NOTIFY airportCodeChanged)
public:
    FlightsModel(Settings *settings,
                 AirportNames *airports,
                 FlightStatuses *statuses,
                 FlightTimes *times,
                 QObject *parent = 0);
    ~FlightsModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool isLoading() const;
    QString airportCode() const;

Q_SIGNALS:
    void loadingChanged();
    void airportCodeChanged();

protected Q_SLOTS:
    void reloadContents();
    void updateContents(Node *root);

protected:
    void appendFlightData(Node *flight);

private:
    Settings *m_settings;
    AirportNames *m_airports;
    FlightStatuses *m_statuses;
    FlightTimes *m_times;

    struct FlightData {
        int m_uniqueID;
        QString m_airline;
        QString m_flightID;
        QString m_domesticOrInternational;
        QDateTime m_scheduleTime;
        QString m_arrivalOrDeparture;
        QString m_airportCode;
        QString m_airportName;
        QStringList m_viaAirportCodes;
        QStringList m_viaAirportNames;
        QString m_routeAirportNames;
        QString m_checkinArea;
        QString m_gate;
        QString m_statusCode;
        QString m_statusText;
        QDateTime m_statusTime;
        QString m_beltNumber;
        bool m_roundtrip;
        QString m_scheduleText;
    };
    QList<FlightData> m_flights;
};

#endif
