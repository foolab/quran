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
            leftMargin: quranTheme.sizes.marginSmall
            rightMargin: quranTheme.sizes.marginSmall
        }

        font.pixelSize: quranTheme.fonts.small
        color: quranTheme.colors.primary
    }

    ComboBox {
        id: combo

        background: Rectangle {
            anchors.fill: parent
            color: combo.pressed ? quranTheme.colors.backgroundHighlight : quranTheme.colors.background
        }

        contentItem: QuranLabel {
            text: combo.displayText
            color: combo.pressed ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
            verticalAlignment: Text.AlignVCenter
            renderType: Text.NativeRendering
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                leftMargin: quranTheme.sizes.marginMedium
                right: parent.right
                rightMargin: quranTheme.sizes.marginMedium
            }
        }

        anchors {
            left: parent.left
            right: parent.right
            leftMargin: quranTheme.sizes.marginSmall
            rightMargin: quranTheme.sizes.marginSmall
        }

        textRole: "text"
        model: actions
        height: quranTheme.sizes.itemSmall
    }
}
