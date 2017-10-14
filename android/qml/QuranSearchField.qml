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

QuranTextField {
    id: control
    property bool enableSoftwareInputPanel

    height: quranTheme.itemSizeLarge
    width: parent.width * 0.75
    placeholderText: qsTr("Search")
    rightPadding: quranTheme.itemSizeSmall

    background: Rectangle {
        anchors.fill: parent
        radius: control.height * 0.16
        border.color: control.activeFocus ? quranTheme.highlightColor : quranTheme.primaryColor
        color: quranTheme.backgroundColor
        Item {
            id: search
            anchors.right: parent.right
            anchors.top: parent.top
            height: parent.height
            width: quranTheme.itemSizeSmall
            ToolButton {
                width: Math.min(parent.width, parent.height)
                height: width
                anchors.centerIn: parent
                highlight: false
                icon: "image://icon/search.png"
            }
        }
    }

//TODO:
//    EnterKey.onClicked: enterKeyClicked()
//    EnterKey.enabled: enterKeyEnabled
//    EnterKey.text: enterKeyText
}
