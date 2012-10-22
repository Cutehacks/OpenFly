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

import QtQuick 1.0

Rectangle {
    id: airports
    color: theme.backgroundColor

    Header {
        id: header
        width: parent.width
        anchors.top: parent.top
        source: "qrc:/images/airport.png"
        text: "Flyplass"
        translatedText: "Airport"
        airportCode: settings.airportCode
    }

    ListView {
        anchors.top: header.bottom
        anchors.topMargin: theme.smallMargin
        anchors.bottom: parent.bottom
        clip: true
        width: parent.width
        model: airportNames.avinorAirports
        delegate: AirportItem { }

        Rectangle {
            height: theme.shadowHeight
            width: parent.width
            anchors.top: parent.top
            gradient: Gradient {
                GradientStop { position: 0.0; color: theme.backgroundColor }
                GradientStop { position: 1.0; color: "transparent" }
            }
        }

        Rectangle {
            height: theme.shadowHeight
            width: parent.width
            anchors.bottom: parent.bottom
            gradient: Gradient {
                GradientStop { position: 0.0; color: "transparent" }
                GradientStop { position: 1.0; color: theme.backgroundColor }
            }
        }
    }
}
