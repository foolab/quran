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

Column {
    id: column

    property color borderColor
    property color backgroundColor
    property alias textColor: label.color
    property alias text: label.text
    property alias font: label.font
    property alias horizontalAlignment: label.horizontalAlignment

    anchors {
        left: parent.left
        right: parent.right
        rightMargin: quranTheme.marginSmall
        leftMargin: quranTheme.marginSmall
    }

    Rectangle {
        width: parent.width
        height: quranTheme.itemSizeDecorative
        color: column.borderColor
    }

    Rectangle {
        width: parent.width
        height: label.height
        color: column.backgroundColor

        QuranLabel {
            id: label
            width: parent.width
        }
    }

    Rectangle {
        width: parent.width
        height: quranTheme.itemSizeDecorative
        color: column.borderColor
    }
}
