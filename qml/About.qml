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
    id: about
    color: theme.backgroundColor

    Header {
        id: header
        width: parent.width
        anchors.top: parent.top
        source: "qrc:/images/info.png"
        text: "Om"
        translatedText: "About"
        airportCode: settings.airportCode
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.verticalCenter
        anchors.bottomMargin: theme.largeMargin
        color: theme.brightTextColor
        wrapMode: Text.WordWrap
        width: parent.width - (theme.doubleMargin * 2)
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: theme.smallFontSize
        text: application.applicationName + " v" + application.applicationVersion + "\nDenne applikasjonen er designet og utviklet av Cutehacks AS"
    }

    BorderImage {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.verticalCenter
        anchors.topMargin: theme.largeMargin
        border.top: 10
        border.bottom: 10
        border.left: 10
        border.right: 10
        source: mouseArea.pressed ? "qrc:/images/border_selected.png" : "qrc:/images/border.png"
        width: 200
        height: 72

        Text {
            anchors.centerIn: parent
            text: "Avslutt"
            color: mouseArea.pressed ? theme.backgroundColor: theme.brightTextColor
            font.pixelSize: theme.mediumFontSize
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: { application.quit(); }
        }
    }

    Image {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: theme.largeMargin
        anchors.right: parent.right
        anchors.rightMargin: theme.largeMargin
        source: "qrc:/images/ninja.png"
    }
}
