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

QuranPopup {
    id: dialog
    signal confirmed
    property string title

    x: parent.x
    y: parent.height - height - quranTheme.sizes.marginMedium

    contentItem: Column {
        id: column
        width: parent.width
        spacing: quranTheme.sizes.spacing

        QuranLabel {
            id: label
            width: parent.width
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: dialog.title
            font.pixelSize: quranTheme.fonts.large
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: quranTheme.sizes.spacing

            QuranButton {
                text: qsTr("No")
                onClicked: dialog.close()
            }

            QuranButton {
                text: qsTr("Yes")
                onClicked: {
                    dialog.close()
                    dialog.confirmed()
                }
            }
        }
    }

    function confirm(message, question) {
        dialog.title = question
        dialog.open()
    }
}
