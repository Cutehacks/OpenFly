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

#include "settings.h"
#include <QApplication>
#include <QDebug>

Settings::Settings(QObject *parent)
    : QSettings(QApplication::organizationName(),
                QApplication::applicationName(),
                parent)
{
}

Settings::~Settings()
{
}

QString Settings::airportCode() const
{
    return value("AirportCode", "OSL").toString();
}

void Settings::setAirportCode(const QString &code)
{
    setValue("AirportCode", code);
    emit airportCodeChanged();
}

int Settings::hoursBefore() const
{
    return value("HoursBefore", 1).toInt();
}

void Settings::setHoursBefore(int hours)
{
    setValue("HoursBefore", hours);
    emit hoursBeforeChanged();
}

int Settings::hoursAfter() const
{
    return value("HoursAfter", 7).toInt();
}

void Settings::setHoursAfter(int hours)
{
    setValue("HoursAfter", hours);
    emit hoursAfterChanged();
}
