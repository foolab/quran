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

Rectangle {
    anchors {
        horizontalCenter: parent.horizontalCenter
        bottom: parent.bottom
        bottomMargin: quranTheme.sizes.marginMedium * 10
    }

    width: parent.width * 0.75
    height: label.height + (2 * quranTheme.sizes.marginSmall)
    color: quranTheme.colors.primary
    opacity: timer.running ? 1.0 : 0.0
    radius: height / 4
    smooth: true

    Behavior on opacity {
        NumberAnimation { duration: quranTheme.animations.medium }
    }

    function showMessage(text) {
        label.text = text
        timer.restart()
    }

    QuranLabel {
        id: label
        anchors {
            bottom: parent.bottom
            right: parent.right
            left: parent.left
            margins: quranTheme.sizes.marginSmall
        }

        verticalAlignment: Text.AlignVCenter
        color: quranTheme.colors.background
        font.bold: true
    }

    Timer {
        id: timer
        interval: 3000
    }
}
