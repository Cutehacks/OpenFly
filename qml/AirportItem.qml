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
    height: 48
    width: parent.width
    property color brightColor: settings.airportCode == airportNames.avinorAirportCodeAt(index) ? theme.foregroundColor : theme.brightTextColor
    color: mouseArea.pressed ? brightColor : theme.backgroundColor
    Text {
        width: parent.width - theme.doubleMargin
        height: parent.height
        anchors.centerIn: parent
        verticalAlignment: Text.AlignVCenter
        text: modelData
        color: mouseArea.pressed ? theme.backgroundColor : brightColor
        font.pixelSize: theme.largeFontSize
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            var airportCode = airportNames.avinorAirportCodeAt(index);
            if (settings.airportCode != airportCode) {
                settings.airportCode = airportCode;
            }
        }
    }
}
