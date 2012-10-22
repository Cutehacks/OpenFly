/*
 *  Copyright (C) 2010 Cutehacks AS. All rights reserved.
 *  info@cutehacks.com
 *  http://cutehacks.com
 *
 */

#ifndef FLIGHTITEM_H
#define FLIGHTITEM_H

#include <QDateTime>
#include <QWidget>
#include "flighttimes.h"

class Node;
class AirportNames;
class FlightStatuses;

class FlightItem : public QWidget
{
    Q_OBJECT
public:
    FlightItem(const QString &airportCode,
               FlightTimes::Direction direction,
               AirportNames *airportNames,
               FlightStatuses *flightStatuses,
               Node *flight,
               QWidget *parent = 0);
    ~FlightItem();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

private:
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
    FlightTimes::Direction m_direction;
    int m_margin;
    bool m_roundtrip;
    bool m_landscape;
};

#endif
