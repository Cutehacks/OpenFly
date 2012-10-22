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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QSettings
{
    Q_OBJECT
    Q_PROPERTY(QString airportCode READ airportCode WRITE setAirportCode NOTIFY airportCodeChanged)
    Q_PROPERTY(int hoursBefore READ hoursBefore WRITE setHoursBefore NOTIFY hoursBeforeChanged)
    Q_PROPERTY(int hoursAfter READ hoursAfter WRITE setHoursAfter NOTIFY hoursAfterChanged)

public:
    Settings(QObject *parent = 0);
    ~Settings();

    QString airportCode() const;
    void setAirportCode(const QString &code);

    int hoursBefore() const;
    void setHoursBefore(int hours);

    int hoursAfter() const;
    void setHoursAfter(int hours);

Q_SIGNALS:
    void airportCodeChanged();
    void hoursBeforeChanged();
    void hoursAfterChanged();
};

#endif
