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

ListDelegate {
    id: root

    property int number
    property alias text: label.text
    property bool highlight
    property alias showPlay: play.visible
    property alias showMore: more.visible
    property alias showNumber: numberLabel.visible

    signal playClicked
    signal moreClicked

    contentHeight: quranTheme.sizes.itemLarge

    Item {
        width: parent.width
        height: quranTheme.sizes.itemLarge

        NumberLabel {
            id: numberLabel
            number: root.number + 1
            width: quranTheme.sizes.itemSmall
            height: width
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            color: root.highlight ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary

            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
        }

        QuranLabel {
            id: label
            height: parent.height
            color: root.highlight ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter

            anchors {
                top: parent.top
                right: numberLabel.visible ? numberLabel.left : parent.right
                left: more.visible ? more.right : play.visible ? play.right : parent.left
            }
        }

        QuranButton {
            id: more
            width: quranTheme.sizes.itemSmall
            height: width
            text: "..."

            anchors {
                left: play.visible ? play.right : parent.left
                verticalCenter: parent.verticalCenter
            }

            onClicked: root.moreClicked()
        }

        ToolButton {
            id: play
            icon: "image://icon/play.png"

            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
            }

            onClicked: root.playClicked()
        }
    }
}
