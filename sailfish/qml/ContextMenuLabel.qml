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
import Sailfish.Silica 1.0

QuranBackgroundItem {
    id: label
    property list<MenuAction> actions
    property alias content: row.children
    property alias contentHeight: row.height

    width: parent.width

    property Item _menu
    property Component _menuComponent: Component {
        ContextMenu {
            Repeater {
                model: label.actions
                MenuItem {
                    text: modelData.text
                    visible: modelData.visible === true
                    onClicked: modelData.clicked()
                }
            }
        }
    }

    onPressAndHold: {
        if (actions.length == 0) {
            return
        }

        if (!_menu) {
            _menu = _menuComponent.createObject(label)
        }

        _menu.show(label)
    }

    height: _menu && _menu.parent == label ? _menu.height + row.height : row.height

    Item {
        id: row
        height: Theme.sizes.itemLarge
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: quranTheme.sizes.marginSmall
        anchors.rightMargin: quranTheme.sizes.marginSmall
    }
}
