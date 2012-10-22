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

// see http://www.avinor.no/avinor/trafikk/50_Flydata for documentation

#include <QApplication>
#include <QDesktopWidget>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include <qplatformdefs.h>

#include "settings.h"
#include "airportnames.h"
#include "flightstatuses.h"
#include "flighttimes.h"
#include "flightsmodel.h"

#include "receiver.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName(QString(QTOSTRING(TARGET)).remove('"'));
    application.setApplicationVersion(QString(QTOSTRING(VERSION)).remove('"'));
    application.setOrganizationDomain("cutehacks.com");
    application.setOrganizationName("Cutehacks");

    QDeclarativeView view;
    RequestQueueThread::setManager(view.engine()->networkAccessManager());

    Settings settings;
    AirportNames airportNames;
    FlightStatuses flightStatuses;
    FlightTimes arrivalTimes(FlightTimes::Arrivals);
    FlightTimes departureTimes(FlightTimes::Departures);
    FlightsModel arrivalsModel(&settings, &airportNames, &flightStatuses, &arrivalTimes);
    FlightsModel departuresModel(&settings, &airportNames, &flightStatuses, &departureTimes);

    QDeclarativeContext *context = view.engine()->rootContext();
    context->setContextProperty("application", &application);
    context->setContextProperty("settings", &settings);
    context->setContextProperty("airportNames", &airportNames);
    context->setContextProperty("flightStatuses", &flightStatuses);
    context->setContextProperty("arrivalsModel", &arrivalsModel);
    context->setContextProperty("departuresModel", &departuresModel);

    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view.setAttribute(Qt::WA_NoSystemBackground);


#if defined(MEEGO_EDITION_HARMATTAN)
    context->setContextProperty("nhdMode", QVariant(false));
    context->setContextProperty("showTime", QVariant(false));
    view.setSource(QUrl("qrc:/qml/MeeGoWindow.qml"));
    view.setGeometry(QApplication::desktop()->screenGeometry());
    view.showFullScreen();
#elif defined(Q_OS_SYMBIAN)
    context->setContextProperty("nhdMode", QVariant(true));
    context->setContextProperty("showTime", QVariant(true));
    view.setSource(QUrl("qrc:/qml/Main.qml"));
    view.showFullScreen();
#else
    context->setContextProperty("nhdMode", QVariant(false));
    context->setContextProperty("showTime", QVariant(false));
    view.setSource(QUrl("qrc:/qml/Main.qml"));
    view.resize(480, 700);
    view.show();
#endif

    QObject::connect(&airportNames, SIGNAL(dataUpdated(Node*)), &flightStatuses, SLOT(start()));
    QObject::connect(&flightStatuses, SIGNAL(dataUpdated(Node*)), &arrivalTimes, SLOT(start()));
    QObject::connect(&flightStatuses, SIGNAL(dataUpdated(Node*)), &departureTimes, SLOT(start()));
    airportNames.start(); // start loading data

    return application.exec();
}
