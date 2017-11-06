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

QtObject {
    property bool inNightMode

    // Application wide colors
    readonly property QtObject colors: QtObject {
        readonly property color primary: inNightMode ? "white" : "black"
        readonly property color secondary: "gray"
        readonly property color background: inNightMode ? "black" : "white"
        readonly property color primaryHighlight: inNightMode ? "lightgray" : "darkgray"
        readonly property color secondaryHighlight: "gray"
        readonly property color backgroundHighlight: inNightMode ? "darkgray" : "lightgray"
    }

    // These are used for quran text only:
    readonly property QtObject quranColors: QtObject {
        readonly property color text: inNightMode ? "white" : "black"
        readonly property color translation: text
        readonly property color background: inNightMode ? "black" : "white"
        readonly property color chapterTitle: text
        readonly property color chapterBorder: "#3f48cc"
        readonly property color chapterBackground: inNightMode ? "gray" : "#bfe8f2"
        readonly property color translationBorder: "#00a3e8"
        readonly property color translationBackground: inNightMode ? "gray" : "#bfe8f2"
        readonly property color highlight: "red"
    }

    readonly property QtObject sizes: QtObject {
        readonly property int decorative: 2
        readonly property int marginSmall: 10
        readonly property int marginMedium: 20
        readonly property int spacing: 5
        readonly property int toolButton: 20
        readonly property int itemSmall: 20
        readonly property int itemLarge: 30
    }

    readonly property QtObject fonts: QtObject {
        readonly property int tiny: 10
        readonly property int small: 20
        readonly property int large: 40
        readonly property int minimum: 15
        readonly property int maximum: 60
        readonly property string heading: "Droid Sans"
    }

    // TODO: get rid of these
    readonly property color keyboardPreviewTextColor: "white"
    readonly property color keyboardPreviewBackgroundColor: "black"

    readonly property string keyboardEnter: "image://theme/icon-m-enter"
    readonly property string keyboardBackspace: "image://theme/icon-m-backspace"
}
