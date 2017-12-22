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
    RecitationsModel {
        id: recitationsModel
        source: recitations
    }

    QuranListView {
        id: view
        anchors.fill: parent

        model: VisibilityFilterModel {
            model: recitationsModel
        }

        section {
            property: "section"
            delegate: QuranLabel {
                anchors {
                    right: parent.right
                    rightMargin: quranTheme.sizes.marginMedium
                    left: parent.left
                    leftMargin: quranTheme.sizes.marginMedium
                }

                color: quranTheme.colors.secondary
                height: quranTheme.sizes.itemSmall
                text: section
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

            Row {
                width: parent.width
                height: quranTheme.sizes.itemLarge
                spacing: quranTheme.sizes.spacing

                QuranLabel {
                    height: parent.height
                    width: parent.width - button.width - quranTheme.sizes.spacing
                    text: recitation.status == Recitation.Installed ? recitation.name : qsTr("%1 %2").arg(recitation.name).arg(recitation.quality)
                    verticalAlignment: Text.AlignVCenter
                    color: item.highlighted ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
                    truncateText: true
                }

                ToolButton {
                    id: button
                    icon: recitation.status == Recitation.Installed ? "image://icon/clear.png" : "image://icon/plus.png"
                    onClicked: _toggleRecitation()
                }
            }

            Item {
                width: parent.width
                height: quranTheme.sizes.itemLarge
                visible: recitation.status == Recitation.Installed

                QuranLabel {
                    height: parent.height
                    width: parent.width - defaultButton.width
                    text: recitation.quality
                    verticalAlignment: Text.AlignVCenter
                    color: item.highlighted ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
                    truncateText: true
                    anchors.left: parent.left
                }

                QuranButton {
                    id: defaultButton
                    visible: settings.defaultRecitation != recitation.uuid
                    anchors {
                        verticalCenter: parent.verticalCenter
                        right: parent.right
                    }
                    text: qsTr("Default")
                    onClicked: recitations.loadAndSetDefault(recitation.uuid)
                }
            }
        }

        header: QuranPageHeader {
            width: parent.width
            title: qsTr("Manage recitations")
        }
    }
}
