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

Item {
    height: theme.largeFontSize + theme.smallMargin
    width: parent.width

    Text {
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: theme.largeMargin
        text: scheduleText
        color: theme.textColor
        font.pixelSize: theme.smallFontSize
    }

    Text {
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: theme.column1
        text: flightID
        color: theme.textColor
        font.pixelSize: theme.smallFontSize
    }

    Text {
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: theme.column2
        text: beltNumber
        color: theme.textColor
        font.pixelSize: theme.smallFontSize
    }

    Text {
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: theme.column3
        text: airportName
        color: theme.textColor
        font.pixelSize: theme.smallFontSize
        width: theme.column4 - theme.column3 - theme.smallMargin
        elide: Text.ElideRight
    }

    Text {
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: theme.column4
        text: statusText
        color: theme.textColor
        font.pixelSize: theme.smallFontSize
    }
}
