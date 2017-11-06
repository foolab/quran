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
import Sailfish.Silica 1.0

BaseTheme {
    readonly property QtObject colors: QtObject {
        readonly property color primary: Theme.colors.primary
        readonly property color secondary: Theme.colors.secondary
        readonly property color background: "transparent"
        readonly property color primaryHighlight: Theme.highlightColor
        readonly property color secondaryHighlight: Theme.colors.secondaryHighlight
        readonly property color backgroundHighlight: Theme.highlightBackgroundColor
    }

    readonly property QtObject sizes: QtObject {
        readonly property int decorative: 2
        readonly property int marginSmall: 10
        readonly property int marginMedium: 16
        readonly property int spacing: 20
        readonly property int toolButton: Theme.itemSizeSmall
        readonly property int itemSmall: Theme.itemSizeSmall
        readonly property int itemLarge: Theme.itemSizeLarge
    }

    readonly property QtObject fonts: QtObject {
        readonly property int tiny: Theme.fontSizeTiny
        readonly property int small: Theme.fontSizeSmall
        readonly property int large: Theme.fontSizeLarge
        readonly property int minimum: Theme.fontSizeTiny
        readonly property int maximum: Theme.fontSizeHuge
        readonly property string heading: Theme.fontFamilyHeading
    }

    // TODO: get rid of these
    readonly property int characterKeySize: Theme.itemSizeExtraSmall
    readonly property bool platformUsableKeyboard: false
}
