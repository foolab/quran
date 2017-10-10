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

QuranBackgroundItem {
    id: topItem
    property alias actions: menu.actions
    default property alias content: item.data
    width: ListView.view.width
    height: contentHeight + menu.menuHeight
    property alias contentHeight: item.height

    onPressAndHold: menu.pressAndHold()
    onClicked: menu.clicked()

    QuranContextMenu {
        id: menu
        container: topItem
    }

    Column {
        id: item

        anchors {
            top: parent.top
            left: parent.left
            leftMargin: quranTheme.marginMedium
            right: parent.right
            rightMargin: quranTheme.marginMedium
        }
    }
}
