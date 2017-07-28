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
import QtQuick.Dialogs 1.2

Item {
    id: item
    property QtObject dialog

    signal confirmed
    property string _question

    Component {
        id: dialogComponent

        MessageDialog {
            text: item._question
            standardButtons: StandardButton.Ok | StandardButton.Cancel
            onAccepted: item.confirmed()
        }
    }

    function confirm(parentItem, message, question) {
        item._question = question

        if (item.dialog == null) {
            item.dialog = dialogComponent.createObject(item)
        }

        item.dialog.open()
    }
}
