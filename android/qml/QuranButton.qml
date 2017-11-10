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

Button {
    id: control
    height: quranTheme.sizes.itemSmall
    background: Rectangle {
        anchors.fill: parent
        color: control.down ? quranTheme.colors.backgroundHighlight : quranTheme.colors.background
        border.width: quranTheme.sizes.decorative
        border.color: control.down ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
        radius: border.width * 2
        implicitWidth: quranTheme.sizes.itemLarge * 2
    }

    contentItem: QuranLabel {
        text: control.text
        color: control.down ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        renderType: Text.NativeRendering
    }
}
