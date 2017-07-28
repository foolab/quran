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
import Quran 1.0

QuranLabel {
    id: label

    property alias number: formatter.number

    text: formatter.formattedNumber

    property bool enableSignals: true
    signal clicked
    signal pressAndHold
    property bool highlight: mouse.pressed && mouse.containsMouse

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: label.clicked()
        onPressAndHold: label.pressAndHold()
        enabled: parent.enableSignals
    }

    NumberFormatter {
        id: formatter
        format: settings.numberFormat
    }

    font.bold: true

    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
}
