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

#ifndef AIRPORTNAMES_H
#define AIRPORTNAMES_H

#include "xmldatasource.h"
#include <QStringList>
#include <QHash>

class AirportNames : public XmlDataSource
{
    Q_OBJECT
    Q_PROPERTY(QStringList avinorAirports READ avinorAirports CONSTANT)
public:
    AirportNames(QObject *parent = 0);
    ~AirportNames();

    QStringList avinorAirports() const;

public Q_SLOTS:
    QString avinorAirportCodeAt(int index) const;
    QString airportName(const QString &code) const;

protected Q_SLOTS:
    void createMapping(Node *root);

private:
    QHash<QString, QString> m_mapping;
    QStringList m_airports;
};

#endif
