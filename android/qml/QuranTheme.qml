// -*- qml -*-
/*
 * Copyright (c) 2011-2017 Mohammed Sameer <msameer@foolab.org>.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import QtQuick.Window 2.2

BaseTheme {
    readonly property int _height: Math.max(Screen.width, Screen.height)

    readonly property QtObject sizes: QtObject {
        readonly property int decorative: 2
        readonly property int marginSmall: _height / 100
        readonly property int marginMedium: _height / 80
        readonly property int spacing: _height / 50
        readonly property int toolButton: _height / 13
        readonly property int itemSmall: _height / 12
        readonly property int itemLarge: _height / 10
    }

    readonly property QtObject fonts: QtObject {
        readonly property int tiny: _height / 45
        readonly property int small: _height / 35
        readonly property int large: _height / 25
        readonly property int minimum: _height / 70
        readonly property int maximum: _height / 10
        readonly property string heading: "Droid Sans"
    }

    readonly property int characterKeySize: 60
    readonly property int toolBarHeight: _height / 12
    readonly property bool platformUsableKeyboard: true
}
