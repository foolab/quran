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
    id: column

    property alias value: slider.value
    property alias minimumValue: slider.from
    property alias maximumValue: slider.to
    property alias stepSize: slider.stepSize
    property alias label: textLabel.text
    property alias pressed: slider.pressed

    QuranLabel {
        id: textLabel

        anchors {
            left: parent.left
            right: parent.right
            leftMargin: quranTheme.marginSmall
            rightMargin: quranTheme.marginSmall
        }

        font.pixelSize: quranTheme.fontSizeSmall
        color: quranTheme.primaryColor
    }

    Slider {
        id: slider

        anchors {
            left: parent.left
            right: parent.right
            leftMargin: quranTheme.marginSmall
            rightMargin: quranTheme.marginSmall
        }

        height: quranTheme.itemSizeSmall
        stepSize: 1.0
        snapMode: Slider.SnapAlways
    }
}
