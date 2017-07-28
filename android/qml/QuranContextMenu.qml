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
import QtQuick.Controls 1.2

Item {
    id: item
    property list<MenuAction> actions
    property QtObject _menu
    property Item container
    property int menuHeight: 0

    property Component _menuComponent: Component {
        Menu {
            id: menu
            Instantiator {
                model: topItem.actions
                delegate: MenuItem {
                    text: modelData.text
                    visible: modelData.visible === true
                    onTriggered: modelData.clicked()
                }

                onObjectAdded: menu.insertItem(index, object)
                onObjectRemoved: menu.removeItem(object)
            }
        }
    }

    function clicked() {
        // Nothing
    }

    function pressAndHold() {
        if (actions.length == 0) {
            return
        }

        var enabledActions = 0
        for (var x = 0; x < actions.length; x++) {
            if (actions[x].visible == true) {
                enabledActions++
                break
            }
        }

        if (enabledActions == 0) {
            return
        }

        if (!_menu) {
            _menu = _menuComponent.createObject(item)
        }

        _menu.popup()
    }
}
