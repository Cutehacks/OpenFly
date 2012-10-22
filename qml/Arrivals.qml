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
    id: arrivals
    color: theme.backgroundColor

    Header {
        id: header
        width: parent.width
        anchors.top: parent.top
        source: "qrc:/images/arrival.png"
        text: "Ankomster"
        translatedText: "Arrivals"
        airportCode: arrivalsModel.airportCode
    }

    DoubleText {
        id: tid
        topText: "Tid"
        bottomText: "Time"
        anchors.top: header.bottom
        anchors.topMargin: theme.mediumMargin
        anchors.left: parent.left
        anchors.leftMargin: theme.largeMargin
    }

    DoubleText {
        id: rute
        topText: "Rute nr."
        bottomText: "Flight no."
        anchors.top: header.bottom
        anchors.topMargin: theme.mediumMargin
        anchors.left: parent.left
        anchors.leftMargin: theme.column1
    }

    DoubleText {
        id: utgang
        topText: "Belte"
        bottomText: "Belt"
        anchors.top: header.bottom
        anchors.topMargin: theme.mediumMargin
        anchors.left: parent.left
        anchors.leftMargin: theme.column2
    }

    DoubleText {
        id: reisemaal
        topText: "Fra"
        bottomText: "From"
        anchors.top: header.bottom
        anchors.topMargin: theme.mediumMargin
        anchors.left: parent.left
        anchors.leftMargin: theme.column3
    }

    DoubleText {
        id: merknader
        topText: "Merknader"
        bottomText: "Remarks"
        anchors.top: header.bottom
        anchors.topMargin: theme.mediumMargin
        anchors.left: parent.left
        anchors.leftMargin: theme.column4
    }

    ListView {
        anchors.top: tid.bottom
        anchors.topMargin: theme.smallMargin
        anchors.bottom: parent.bottom
        width: parent.width
        clip: true
        model: arrivalsModel
        header: Item { height: theme.shadowHeight }
        delegate: ArrivalItem { }

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

    Loading {
        anchors.centerIn: parent
        visible: arrivalsModel.loading
    }
}
