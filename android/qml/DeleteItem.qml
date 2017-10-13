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

        Popup {
            id: dialog
            property string title
            x: 0
            y: 0
            modal: true
            focus: true
            width: parent.width
            height: parent.height

            enter: Transition {
                NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 100 }
            }

            exit: Transition {
                NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 100 }
            }

            Row {
                spacing: quranTheme.spacing
                width: parent.width
                height: parent.height

                QuranLabel {
                    width: parent.width - 2 * (quranTheme.spacing + quranTheme.itemSizeSmall)
                    height: parent.height
                    text: dialog.title
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                QuranButton {
                    anchors.verticalCenter: parent.verticalCenter
                    width: quranTheme.itemSizeSmall
                    height: quranTheme.itemSizeSmall
                    text: qsTr("No")
                    onClicked: {
                        dialog.close()
                        dialog.destroy(100)
                    }
                }

                QuranButton {
                    anchors.verticalCenter: parent.verticalCenter
                    width: quranTheme.itemSizeSmall
                    height: quranTheme.itemSizeSmall
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
