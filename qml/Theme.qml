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

QtObject {
    id: theme

    property bool inPortrait: true

    property color backgroundColor: "#000000"
    property color foregroundColor: "#FFFFFF"
    property color textColor: "#FCD116"
    property color brightTextColor: "#DD7500"

    // Symbian: 640 x 360
    // Harmattan: 854 x 480

    property real factor: nhdMode ? 1 : 1.33

    property int shadowHeight: 16
    property int headerHeight: 32 * factor
    property int footerHeight: 32 * factor
    property int smallFontSize: /*14 * factor */(inPortrait ? 14 : 18) * factor
    property int mediumFontSize: 18 * factor
    property int largeFontSize: 22 * factor
    property int smallMargin: 4 * factor
    property int mediumMargin: 8 * factor
    property int largeMargin: 12 * factor
    property int doubleMargin: 24 * factor

    property int column1: /*58 * factor*/(inPortrait ? 58 : 100) * factor
    property int column2: /*130 * factor*/(inPortrait ? 130 : 200) * factor
    property int column3: /*170 * factor*/(inPortrait ? 170 : 300) * factor
    property int column4: /*260 * factor*/(inPortrait ? 260 : 500) * factor
}
