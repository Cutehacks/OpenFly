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
    id: header
    height: theme.headerHeight
    property string source
    property string text
    property string translatedText
    property string airportCode

    Image {
        id: icon
        source: parent.source
        x: theme.largeMargin
        y: theme.smallMargin
    }

    Text {
        id: primary
        text: parent.text
        color: theme.foregroundColor
        font.pixelSize: theme.largeFontSize
        height: parent.height
        anchors.top: parent.top
        anchors.left: icon.right
        anchors.leftMargin: theme.mediumMargin
    }

    Text {
        id: secondary
        text: parent.translatedText
        color: theme.brightTextColor
        font.pixelSize: theme.largeFontSize
        height: parent.height
        anchors.top: parent.top
        anchors.left: primary.right
        anchors.leftMargin: theme.mediumMargin
    }

    Text {
        id: airport
        text: airportCode
        color: theme.foregroundColor
        font.pixelSize: theme.largeFontSize
        height: parent.height
        anchors.top: parent.top
        anchors.right: time.visible ? time.left : parent.right
        anchors.rightMargin: theme.mediumMargin
    }

    Text {
        id: time
        visible: showTime
        text: Qt.formatDateTime(new Date(), "hh:mm")
        color: theme.foregroundColor
        font.pixelSize: theme.largeFontSize
        height: parent.height
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: theme.mediumMargin
        Timer {
            interval: 3000
            running: parent.visible
            repeat: true
            onTriggered: time.text = Qt.formatDateTime(new Date(), "hh:mm")
        }
    }
}
