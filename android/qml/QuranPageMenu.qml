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

Menu {
    id: menu
    property list<MenuAction> actions
    property variant view
    x: parent.width - width
    modal: true

    Instantiator {
        function reverse(m) {
            var l = m.length
            var out = []

            for (var x = 0; x < l; x++) {
                out[x] = m[l - x - 1]
            }

            return out
        }

        model: reverse(actions)

        delegate: MenuItem {
            text: modelData.text
            onTriggered: modelData.clicked()
        }

        onObjectAdded: menu.insertItem(index, object)
        onObjectRemoved: menu.removeItem(object)
    }
}
