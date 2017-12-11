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

QuranBackgroundItem {
    property alias text: label.text

    function _color(index) {
        if (item.currentIndex == index) {
            return highlighted ? quranTheme.colors.secondaryHighlight : quranTheme.colors.secondary
        } else {
            return highlighted ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
        }
    }

    width: parent.width
    height: quranTheme.sizes.itemSmall
    onClicked: {
        item.currentIndex = index
        popup.close()
        popup.destroy(200)
    }

    QuranLabel {
        id: label

        anchors {
            fill: parent
            margins: quranTheme.sizes.marginSmall
        }

        verticalAlignment: Text.AlignVCenter
        color: parent._color(index)
    }
}
