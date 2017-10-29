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

    property color buttonNormalColor: inNightMode ? "white" : "black"
    property color buttonHighlightColor: highlightColor

    property int marginSmall: _height / 100
    property int marginMedium: _height / 80
    property int spacing: _height / 50
    property int toolButtonSize: _height / 13

    property int fontSizeSmall: _height / 35
    property int fontSizeLarge: _height / 25
    property color primaryColor: inNightMode ? "white" : "black"
    property string fontFamilyHeading: "Nokia Pure Text"
    property int itemSizeSmall: _height / 12
    property int itemSizeLarge: _height / 10
    property int characterKeySize: 60
    property color highlightColor: "gray"

    property int toolBarHeight: _height / 12
    property bool platformUsableKeyboard: true

    property int minimumFontSize: _height / 70
    property int maximumFontSize: _height / 10
    property int minimumTranslationFontSize: minimumFontSize
    property int maximumTranslationFontSize: maximumFontSize
}
