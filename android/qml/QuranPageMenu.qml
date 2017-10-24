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
    y: parent.height - menu.height
    modal: true
    // StackView has the focus and it handles the android back key navigation
    // It will lose focus when the menu is opened. We need to restore the focus
    // again otherwise a back key press will quit the application
    onVisibleChanged: {
        if (!visible) {
            root.pageStack.focus = true
        }
    }
    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 100 }
    }

    exit: Transition {
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 100 }
    }

    Instantiator {
        model: actions

        delegate: MenuItem {
            text: modelData.text
            onTriggered: modelData.clicked()
        }

        onObjectAdded: menu.insertItem(index, object)
        onObjectRemoved: menu.removeItem(object)
    }
}
