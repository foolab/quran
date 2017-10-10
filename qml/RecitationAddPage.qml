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
import Quran 1.0

QuranPage {
    QuranListView {
        id: view
        anchors.fill: parent
        model: recitations

        section {
            property: "online"
            delegate: QuranLabel {
                anchors {
                    right: parent.right
                    rightMargin: quranTheme.marginMedium
                    left: parent.left
                    leftMargin: quranTheme.marginMedium
                }

                color: quranTheme.highlightColor
                height: quranTheme.itemSizeSmall
                text: section == "online" ? qsTr("Online recitations") : qsTr("Offline recitations")
                verticalAlignment: Text.AlignVCenter
            }
        }

        delegate: ListDelegate {
            id: item
            function _toggleRecitation() {
                if (recitation.status != Recitation.Installed) {
                    if (!recitation.enable()) {
                        banner.showMessage("Failed to enable recitation")
                    }
                } else {
                    if (!recitation.disable()) {
                        banner.showMessage("Failed to disable recitation")
                    }
                }
            }

            QuranTextSwitch {
                id: _switch
                height: quranTheme.itemSizeLarge
                text: qsTr("%1 %2").arg(recitation.name).arg(recitation.quality)
                checked: recitation.status == Recitation.Installed
                checkable: false
                onClicked: _toggleRecitation()
            }
        }

        header: QuranPageHeader {
            width: parent.width
            title: qsTr("Manage recitations")
        }
    }
}
