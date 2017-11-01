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
    property QtObject textSupplier
    property int verse
    width: parent.width

    signal clicked

    font.family: settings.fontFamily
    font.pixelSize: settings.fontSize
    color: quranTheme.colors.primary
    horizontalAlignment: settings.centerText ? Text.AlignHCenter : Text.AlignHRight

    NumberFormatter {
        id: formatter
        format: settings.numberFormat
        number: verse + 1
    }

    MouseArea {
        anchors.fill: parent
        onClicked: label.clicked();
    }

    text: qsTr("%1 (%2)").arg(textSupplier.primaryText).arg(formatter.formattedNumber)
}
