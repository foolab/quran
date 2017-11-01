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

MouseArea {
    property string text
    property url icon
    property bool highlight: pressed && containsMouse
    property bool highlightEnabled: true
    width: quranTheme.sizes.toolButton
    height: width
    anchors.verticalCenter: parent ? parent.verticalCenter : undefined

    function _icon() {
        if (highlight) {
            return icon + "?" + quranTheme.colors.primaryHighlight
        } else {
            return icon + "?" + quranTheme.colors.primary
        }
    }

    Image {
        id: image
        width: parent.width * 0.65
        height: parent.height * 0.65
        anchors.centerIn: parent
        opacity: parent.enabled ? 1.0 : 0.5
        source: parent.highlightEnabled ? _icon() : parent.icon
    }
}
