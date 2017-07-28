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

ToolButton {
    property QuranPageMenu menu

    height: parent.height
    width: height

    onClicked: {
        if (menu) {
            menu.popup()
        }
    }

    visible: parent.visible && menu != null
    opacity: visible ? 1.0 : 0
    icon.source: highlight ? "image://icon/menu.png?" + quranTheme.buttonHighlightColor : "image://icon/menu.png?" + quranTheme.buttonNormalColor

    Behavior on opacity {
        PropertyAnimation { duration: 400 }
    }
}
