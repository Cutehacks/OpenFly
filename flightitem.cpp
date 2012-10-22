/*
 *  Copyright (C) 2010 Cutehacks AS. All rights reserved.
 *  info@cutehacks.com
 *  http://cutehacks.com
 *
 */

#include "flightitem.h"
#include "airportnames.h"
#include "flightstatuses.h"
#include "node.h"
#include "constants.h"

#include <QDateTime>
#include <QPainter>
#include <QMouseEvent>

FlightItem::FlightItem(const QString &airportCode,
                       FlightTimes::Direction direction,
                       AirportNames *airportNames,
                       FlightStatuses *flightStatuses,
                       Node *flight,
                       QWidget *parent)
    : QWidget(parent), m_direction(direction), m_margin(4), m_roundtrip(false), m_landscape(false)
{
    setFixedHeight(pixelSizes[0][1] + m_margin);
    if (flight && flight->type() == "flight") {
        m_uniqueID = flight->attribute("uniqueID").toLong();
        if (Node *airline = flight->firstChildOfType("airline"))
            m_airline = airline->text();
        if (Node *flight_id = flight->firstChildOfType("flight_id"))
            m_flightID = flight_id->text();
        if (Node *dom_int = flight->firstChildOfType("dom_int"))
            m_domesticOrInternational = dom_int->text(); // "D" = domestic, "I" = international
        if (Node *schedule_time = flight->firstChildOfType("schedule_time")) {
            QDateTime scheduleTime = QDateTime::fromString(schedule_time->text(), Qt::ISODate);
	    scheduleTime.setTimeSpec(Qt::UTC);
            m_scheduleTime = scheduleTime.toLocalTime();
        }
        if (Node *arr_dep = flight->firstChildOfType("arr_dep"))
            m_arrivalOrDeparture = arr_dep->text(); // "D" = departure, "A" = arrival
        if (Node *airport = flight->firstChildOfType("airport")) {
            m_airportCode = airport->text();
            m_airportName = airportNames ? airportNames->airportName(m_airportCode) : QString();
            m_roundtrip = (m_airportCode == airportCode); // if destination and origin are the same
        }
        if (Node *via_airport = flight->firstChildOfType("via_airport")) {
            m_viaAirportCodes = via_airport->text().split(',');
            foreach(QString code, m_viaAirportCodes)
                m_viaAirportNames.append(airportNames->airportName(code));
            if (!m_roundtrip && m_arrivalOrDeparture == "D")
                m_viaAirportNames.append(m_airportName);
            m_routeAirportNames = m_viaAirportNames.join(", ");
        }
        if (Node *check_in = flight->firstChildOfType("check_in"))
            m_checkinArea = check_in->text(); // "A" "B" "C" "D" checking area
        if (Node *gate = flight->firstChildOfType("gate"))
            m_gate = gate->text();
        if (Node *status = flight->firstChildOfType("status")) {
            m_statusCode = status->attribute("code");
            m_statusText = flightStatuses ? flightStatuses->statusTextNorwegian(m_statusCode) : QString(); // ### support norwegian
            QDateTime statusTime = QDateTime::fromString(status->attribute("time"), Qt::ISODate);
	    statusTime.setTimeSpec(Qt::UTC);
            m_statusText += statusTime.toLocalTime().toString(" hh:mm");
        }
        if (Node *belt_number = flight->firstChildOfType("belt_number"))
            m_beltNumber = belt_number->text();
    }
}

FlightItem::~FlightItem()
{
}

void FlightItem::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QWidget *view = parentWidget() ? parentWidget()->parentWidget() : 0;
    m_landscape = view && (view->width() > view->height());
    const int i = (m_landscape ? 1 : 0);
    setFixedHeight(pixelSizes[i][1] + m_margin);
    QFont fnt(font());
    fnt.setPixelSize(pixelSizes[i][0]);
    setFont(fnt);
}

void FlightItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    const int i = (m_landscape ? 1 : 0);
    painter.setPen(palette().color(QPalette::BrightText));
    painter.drawText(QRect(columns[i][0] - 2, 0, columns[i][1] - columns[i][0], height()), m_scheduleTime.time().toString("hhmm"));
    painter.drawText(QRect(columns[i][1] - 2, 0, columns[i][2] - columns[i][1], height()), m_flightID);
    if (m_direction == FlightTimes::Arrivals)
        painter.drawText(QRect(columns[i][2] - 2, 0, columns[i][3] - columns[i][2], height()), m_beltNumber);
    else
        painter.drawText(QRect(columns[i][2] - 2, 0, columns[i][3] - columns[i][2], height()), m_gate);
    QTextOption option;
    option.setWrapMode(QTextOption::NoWrap);
    const QString name = (m_routeAirportNames.isEmpty() ? m_airportName : m_routeAirportNames);
    int last = width() > columns[i][5] ? columns[i][5] : columns[i][4];
    painter.drawText(QRect(columns[i][3] - 2, 0,  last - columns[i][3], height()), fontMetrics().elidedText(name, Qt::ElideRight, last - columns[i][3]), option);
    painter.drawText(QRect(last - 2, 0, width() - last, height()), m_statusText, option);
}
