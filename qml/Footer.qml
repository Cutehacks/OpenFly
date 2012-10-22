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
    id: footer
    height: theme.footerHeight
    property int currentPage

    Text {
        id: avinor
        text: "Flydata fra Avinor"
        color: theme.foregroundColor
        font.pixelSize: theme.smallFontSize
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: theme.mediumMargin

        MouseArea {
            anchors.fill: parent
            onClicked: { Qt.openUrlExternally("http://www.avinor.no"); }
        }
    }

    Text {
        id: cutehacks
        text: "Cutehacks AS"
        color: theme.foregroundColor
        font.pixelSize: theme.smallFontSize
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: theme.mediumMargin

        MouseArea {
            anchors.fill: parent
            onClicked: { Qt.openUrlExternally("http://cutehacks.com"); }
        }
    }

    Item {
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height
        Row {
            anchors.centerIn: parent
            Dot { active: currentPage < 1 }
            Dot { active: currentPage >= 1 && currentPage < 2 }
            Dot { active: currentPage >= 2 && currentPage < 3 }
            Dot { active: currentPage >= 3 }
        }
    }
}
