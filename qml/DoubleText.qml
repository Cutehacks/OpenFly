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
    property string topText
    property string bottomText
    height: upper.paintedHeight + theme.smallMargin + lower.paintedHeight
    width: Math.max(upper.paintedWidth, lower.paintedWidth)

    Text {
        id: upper
        text: parent.topText
        color: theme.foregroundColor
        font.pixelSize: theme.smallFontSize
        anchors.top: parent.top
        anchors.left: parent.left
    }

    Text {
        id: lower
        text: parent.bottomText
        color: theme.brightTextColor
        font.pixelSize: theme.smallFontSize
        anchors.top: upper.bottom
        anchors.topMargin: theme.smallMargin
        anchors.left: parent.left
    }
}
