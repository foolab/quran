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

Rectangle {
    anchors {
        horizontalCenter: parent.horizontalCenter
        top: parent.top
    }

    width: parent.width
    height: Theme.itemSizeSmall
//    color: Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
    color: Theme.highlightBackgroundColor
    opacity: timer.running ? 1.0 : 0.0
    Behavior on opacity {
        NumberAnimation { duration: 250 }
    }

    function showMessage(text) {
        label.text = text
        timer.restart()
    }

    QuranLabel {
        id: label
        width: parent.width
        height: parent.height
        verticalAlignment: Text.AlignVCenter
        color: Theme.primaryColor
        truncateText: true
    }

    Timer {
        id: timer
        interval: 3000
    }
}
