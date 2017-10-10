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

import QtQuick 2.2
import QtQuick.Controls 2.2

Column {
    property list<MenuAction> actions
    property alias label: text.text
    property alias currentIndex: combo.currentIndex

    width: parent.width

    QuranLabel {
        id: text

        anchors {
            left: parent.left
            right: parent.right
            leftMargin: quranTheme.marginSmall
            rightMargin: quranTheme.marginSmall
        }

        font.pixelSize: quranTheme.fontSizeSmall
        color: quranTheme.primaryColor
    }

    ComboBox {
        id: combo

        background: Rectangle {
            function _color() {
                if (combo.pressed) {
                    return quranTheme.inNightMode ? "#191919" : "#e5e5e5"
                } else {
                    return quranTheme.backgroundColor
                }
            }

            anchors.fill: parent
            color: _color()
        }

        contentItem: QuranLabel {
            text: combo.displayText
            color: quranTheme.primaryColor
            verticalAlignment: Text.AlignVCenter
            renderType: Text.NativeRendering
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                leftMargin: quranTheme.marginMedium
                right: parent.right
                rightMargin: quranTheme.marginMedium
            }
        }

        anchors {
            left: parent.left
            right: parent.right
            leftMargin: quranTheme.marginSmall
            rightMargin: quranTheme.marginSmall
        }

        textRole: "text"
        model: actions
        height: quranTheme.itemSizeSmall
    }
}
