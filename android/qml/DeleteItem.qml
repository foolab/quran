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

Item {
    id: item
    width: parent.width
    height: 0

    signal confirmed

    Component {
        id: component

        QuranPopup {
            id: dialog
            property string title
            x: 0
            y: 0

            height: parent.height

            Row {
                spacing: quranTheme.sizes.spacing
                width: parent.width
                height: parent.height

                QuranLabel {
                    width: parent.width - 2 * (quranTheme.sizes.spacing + quranTheme.sizes.itemSmall)
                    height: parent.height
                    text: dialog.title
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                QuranButton {
                    anchors.verticalCenter: parent.verticalCenter
                    width: quranTheme.sizes.itemSmall
                    height: quranTheme.sizes.itemSmall
                    text: qsTr("No")
                    onClicked: {
                        dialog.close()
                        dialog.destroy(100)
                    }
                }

                QuranButton {
                    anchors.verticalCenter: parent.verticalCenter
                    width: quranTheme.sizes.itemSmall
                    height: quranTheme.sizes.itemSmall
                    text: qsTr("Yes")
                    onClicked: {
                        dialog.close()
                        item.confirmed()
                        dialog.destroy(100)
                    }
                }
            }
        }
    }

    function confirm(parentItem, message, question) {
        var obj = component.createObject(parentItem, {"title": question})
        obj.open()
    }
}
