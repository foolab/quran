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
    menu: pageMenu

    QuranPageMenu {
        id: pageMenu
        view: view
        actions: [
            MenuAction {
                text: qsTr("Manage recitations")
                icon: "image://icon/recitation-settings.png"
                onClicked: pushAnimated(Qt.resolvedUrl("RecitationAddPage.qml"))
            }
        ]
    }

    QuranListView {
        id: view
        anchors.fill: parent

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

        QuranViewPlaceholder {
            enabled: recitations.installedCount == 0 || settings.recitationMode == 0
            text: settings.recitationMode == 0 ?
                qsTr("Enable recitations from application settings.") :
                qsTr("Enable recitations by selecting 'Manage recitations' from the menu.")
        }

        header: QuranPageHeader {
            width: parent.width
            title: qsTr("Choose recitation")
        }

        InstalledFilterModel {
            id: recitationsModel
            Component.onCompleted: init(recitations, "status", Recitation.Installed, "recitation")
        }


        model: settings.recitationMode != 0 ? recitationsModel : 0

        delegate: ListDelegate {
            id: item

            onClicked: {
                if (recitations.loadRecitation(recitation.uuid)) {
                    settings.defaultRecitation = recitation.uuid
                    popPage()
                } else {
                    banner.showMessage(qsTr("Failed to load recitation"))
                }
            }

            QuranLabel {
                width: parent.width
                height: quranTheme.itemSizeLarge
                truncateText: true
                text: qsTr("%1 %2").arg(recitation.name).arg(recitation.quality)
                verticalAlignment: Text.AlignVCenter
                color: recitation.loaded ? quranTheme.highlightColor : quranTheme.primaryColor
            }
        }
    }
}
