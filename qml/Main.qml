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
    id: main
    color: theme.backgroundColor
    property bool inPortrait: true
    //property bool inPortrait: (height > width)

    Theme {
        id: theme
        inPortrait: main.inPortrait
    }

    Footer {
        id: footer
        width: parent.width
        anchors.bottom: parent.bottom
        currentPage: pages.contentX / main.width
    }

    VisualItemModel {
        id: screens

        Airports {
            id: airports
            x: 0
            width: main.width
            height: main.height - footer.height
        }

        Departures {
            id: departures
            x: main.width
            width: main.width
            height: main.height - footer.height
        }

        Arrivals {
            id: arrivals
            x: main.width * 2
            width: main.width
            height: main.height - footer.height
        }

        About {
            id: about
            x: main.width * 3
            width: main.width
            height: main.height - footer.height
        }

    }

    ListView {
        id: pages
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: footer.top
        model: screens
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        contentX: width
    }

    NumberAnimation {
        id: anim
        target: pages
        property: "contentX"
        to: pages.width
    }

    Connections {
        target: settings
        onAirportCodeChanged: anim.start();
    }

}
