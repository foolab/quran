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

            actions: [
                MenuAction {
                    text: qsTr("Enable")
                    onClicked: {
                        if (!recitation.enable()) {
                            banner.showMessage("Failed to enable recitation")
                        }
                    }

                    visible: recitation.type == Recitation.Online && recitation.status != Recitation.Installed
                },
                MenuAction {
                    text: qsTr("Disable")
                    onClicked: {
                        if (!recitation.disable()) {
                            banner.showMessage("Failed to disable recitation")
                        }
                    }

                    visible: recitation.type == Recitation.Online && recitation.status == Recitation.Installed
                }
            ]

            Item {
                width: parent.width
                height: quranTheme.itemSizeLarge

                QuranStatusIndicator {
                    id: indicator
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    isInstalled: recitation.status == Recitation.Installed
                    downloading: false
                    error: false
                }

                QuranLabel {
                    id: label
                    text: qsTr("%1 %2").arg(recitation.name).arg(recitation.quality)
                    color: quranTheme.primaryColor
                    truncateText: true
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        right: indicator.left
                        rightMargin: quranTheme.marginSmall
                        left: parent.left
                    }

                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        header: QuranPageHeader {
            width: parent.width
            title: qsTr("Manage recitations")
        }
    }
}
