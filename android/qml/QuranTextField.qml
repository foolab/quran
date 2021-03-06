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

TextField {
    id: control
    property bool enterKeyEnabled: true
    property string enterKeyText

    signal enterKeyClicked

    height: quranTheme.sizes.itemSmall
    Keys.onReturnPressed: enterKeyClicked()
    color: quranTheme.colors.primary
// TODO: find a better way
    Component.onCompleted: {
        // I cannot find a way to change the color of the placeholder text except by this hack
        for (var x = 0; x < children.length; x++) {
            if (children[x].text == control.placeholderText) {
                children[x].color = Qt.binding(function() { return quranTheme.colors.secondary })
            }
        }
    }

    background: Rectangle {
        anchors.fill: parent
        radius: control.height * 0.16
        border.color: control.activeFocus ? quranTheme.colors.secondary : quranTheme.colors.primary
        color: quranTheme.colors.background
    }

//TODO:
//    EnterKey.onClicked: enterKeyClicked()
//    EnterKey.enabled: enterKeyEnabled
//    EnterKey.text: enterKeyText
}
