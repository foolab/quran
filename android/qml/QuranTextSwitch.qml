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

Item {
    property alias checked: textSwitch.checked
    property alias text: label.text
    property alias checkable: textSwitch.checkable
    signal clicked()

    anchors {
        left: parent.left
        right: parent.right
        leftMargin: quranTheme.marginSmall
        rightMargin: quranTheme.marginSmall
    }

    height: quranTheme.itemSizeSmall

    QuranLabel {
        id: label
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: textSwitch.left
            left: parent.left
        }

        verticalAlignment: Text.AlignVCenter
        color: quranTheme.primaryColor
    }

    Switch {
        id: textSwitch
        onClicked: parent.clicked()

        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
    }
}
